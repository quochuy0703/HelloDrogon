#ifndef MAP_JSON_HPP
#define MAP_JSON_HPP

#include <iostream>
#include <boost/pfr/core.hpp>
#include <boost/pfr/core_name.hpp>
#include <map>
#include <string>
#include <algorithm>
#include <optional>
#include <type_traits>
#include <typeindex>
#include "ArrayVector.hpp"
#include <boost/type_index.hpp>
#include <chrono>
#include <json/json.h>
#include <drogon/drogon.h>

using namespace drogon::orm;

// sử dụng inline cho những hàm không phải template, vì chỉ sử dụng một file .hpp để define hàm nếu không sẽ bị lỗi
//  multiple definition, first defined here errors
namespace app_helpers::map_json
{
    template <typename T>
    std::string to_string_generic(const T &value);

    template <typename T>
    std::string getPrimaryKey()
    {
        std::string result = std::string{};
        constexpr auto names = boost::pfr::names_as_array<T>();
        T fieldB;
        boost::pfr::for_each_field(
            fieldB,
            [&](auto &field, const std::size_t idx)
            {
                std::string name = std::string(names[idx]);
                typename std::remove_reference<decltype(field)>::type tmp1;
                if constexpr (std::is_same_v<decltype(tmp1), std::string>)
                {
                    if (name == "__primaryKey__")
                        result = field;
                }
            });
        return result;
    }

    template <typename base_type>
    std::string get_type()
    {
        // std::decay_t sẽ loại bỏ &, &&, và const, giúp bạn hiển thị tên loại gốc của biến mà không có các ký hiệu này
        // Để hiển thị kiểu dữ liệu rõ ràng hơn, bạn có thể sử dụng thư viện như boost::typeindex
        return boost::typeindex::type_id_with_cvr<std::decay_t<base_type>>().pretty_name();
    }

    template <typename T>
    std::string getItemHash(T value, std::string parentHash, std::string prefixName, std::string primaryKey = std::string{""})
    {
        std::string result = std::string{""};

        if (primaryKey == "")
        {
            primaryKey = getPrimaryKey<T>();
        }

        constexpr auto names = boost::pfr::names_as_array<T>();
        const std::string nameType = get_type<T>();
        boost::pfr::for_each_field(
            value,
            [&](auto &field, std::size_t idx)
            {
                const std::string name = std::string(names[idx]);
                typename std::remove_reference<decltype(field)>::type tmp;
                if (std::is_arithmetic_v<decltype(tmp)>)
                {
                    if (name == primaryKey)
                    {
                        result = parentHash + prefixName + to_string_generic(field);
                    }
                }
            });

        return result;
    }

    // Extracts the prefix of a key (before the last dot)
    inline std::string keyPrefixString(const std::string &key)
    {
        auto pos = key.find_last_of('.');
        return (pos != std::string::npos) ? key.substr(0, pos) : "";
    }

    // Removes the prefix from the key (after the last dot)
    inline std::string removeKeyPrefix(const std::string &key)
    {
        auto pos = key.find_last_of('.');
        return (pos != std::string::npos) ? key.substr(pos + 1) : key;
    }

    // Splits a string by '.' into an array of strings
    inline std::vector<std::string> keyPrefix(const std::string &key)
    {
        std::vector<std::string> result;
        std::stringstream ss(key);
        std::string segment;
        while (std::getline(ss, segment, '.'))
        {
            result.push_back(segment);
        }
        return result;
    }

    // Extract the last key prefix
    inline std::string lastKeyPrefix(const std::string &key)
    {
        auto parts = keyPrefix(key);
        return (!parts.empty()) ? parts.back() : "";
    }

    inline std::string setNameFeild(const std::string &inputString, const std::string &insertString);

    template <typename T>
    struct DefaultValue
    {
        static T value() { return T(); }
    };

    // loại bỏ namespace
    inline std::string remove_namespace(const std::string &type_name)
    {
        std::size_t pos = type_name.find_last_of("::");
        if (pos != std::string::npos)
        {
            return type_name.substr(pos + 1);
        }
        return type_name;
    }

    // check optional
    template <typename T>
    struct is_optional : std::false_type
    {
    };

    template <typename T>
    struct is_optional<std::optional<T>> : std::true_type
    {
    };

    // remove optional
    template <typename T>
    struct remove_optional
    {
        using type = T;
    };

    template <typename T>
    struct remove_optional<std::optional<T>>
    {
        using type = T;
    };

    // Lấy loại phần tử của vector
    // std::remove_reference loại bỏ tham chiếu (&).
    // std::remove_cv loại bỏ tính chất const và volatile.
    // T::value_type lấy loại phần tử của std::vector.
    // sử dụng :
    // std::vector<int> vec;
    // remove_vector_t<decltype(vec)> elementType;
    template <typename T>
    using remove_vector_t = typename std::remove_cv<
        typename std::remove_reference<
            typename T::value_type>::type>::type;

    // kiem tra vector
    template <typename T>
    struct is_vector : std::false_type
    {
    };

    template <typename T, typename Alloc>
    struct is_vector<std::vector<T, Alloc>> : std::true_type
    {
    };

    template <typename T>
    struct is_std_vector : std::false_type
    {
    };

    template <typename T>
    struct is_std_vector<std::vector<T>> : std::true_type
    {
    };

    template <typename T>
    constexpr bool is_struct_v = std::is_class<T>::value && !std::is_union<T>::value;

    template <typename T>
    void setObject(T &obj, std::map<std::string, std::string> json)
    {
        // constexpr auto names = boost::pfr::names_as_array<T>();
        typename std::remove_reference<decltype(obj)>::type tmp;
        constexpr auto names = boost::pfr::names_as_array<decltype(tmp)>();
        boost::pfr::for_each_field(
            obj,
            [&](auto &field, std::size_t idx)
            {
                const std::string name = std::string(names[idx]);
                field = json[name];
            });
    }

    template <typename T>
    void printObject(const T &obj)
    {
        typename std::remove_reference<decltype(obj)>::type tmp;
        constexpr auto names = boost::pfr::names_as_array<decltype(tmp)>();
        // constexpr auto names = boost::pfr::names_as_array<T>();
        boost::pfr::for_each_field(
            obj,
            [&names](const auto &field, std::size_t idx)
            { std::cout << idx << ": " << names[idx] << " = " << field << '\n'; });
    }

    template <typename T>
    void assignFields(T &fieldB, const Json::Value &row, const std::string &prefix)
    {
        constexpr auto names = boost::pfr::names_as_array<T>();
        boost::pfr::for_each_field(
            fieldB,
            [&](auto &field, std::size_t idx)
            {
                const std::string name = std::string(names[idx]);
                auto temp = field;
                field = row[setNameFeild(name, prefix)].template as<decltype(temp)>();
            });
    }

    // dùng hàm printField
    // Lỗi này xảy ra khi bạn đang cố gắng xuất một kiểu dữ liệu mà không có toán tử << được định nghĩa cho kiểu đó.
    // Điều này thường xảy ra khi bạn đang cố gắng xuất giá trị của một biến mà không hỗ trợ việc xuất thông qua std::ostream.

    // Để khắc phục, bạn có thể kiểm tra xem kiểu dữ liệu có hỗ trợ toán tử << không trước khi cố gắng xuất nó.
    // Nếu kiểu dữ liệu không hỗ trợ, bạn có thể cung cấp cách xử lý riêng hoặc đơn giản là không xuất biến đó
    template <typename T>
    std::ostream &printField(std::ostream &os, const T &field)
    {
        if constexpr (requires(std::ostream &os, const T &value) { os << value; })
        {
            os << field;
        }
        else
        {
            os << "[cannot print this type]";
        }
        return os;
    }

    template <typename T>
    void printObjectExt(T &obj)
    {
        if constexpr (std::is_class_v<T> && !std::is_same_v<T, std::string>)
        {
            typename std::remove_reference<decltype(obj)>::type tmp;
            constexpr auto names = boost::pfr::names_as_array<decltype(tmp)>();
            // constexpr auto names = boost::pfr::names_as_array<T>();
            boost::pfr::for_each_field(
                obj,
                [&names](auto &field, std::size_t idx)
                {
                    const std::string name = std::string(names[idx]);
                    typename std::remove_reference<decltype(field)>::type tmp1;
                    if constexpr (is_vector<decltype(tmp1)>::value)
                    {
                        auto size = field.size();
                        using NonVectorType = remove_vector_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonVectorType> && !std::is_same<NonVectorType, std::string>::value)
                        {
                            std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << std::endl;
                            for (auto o : field)
                            {
                                printObjectExt(o);
                            }
                        }
                        else
                        {
                            std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << "[";
                            int n = 0;
                            for (auto o : field)
                            {
                                printField(std::cout, o);
                                n++;
                                if (n != field.size())
                                {
                                    std::cout << ",";
                                }
                            }

                            std::cout << "]" << std::endl;
                        }
                    }
                    else if constexpr (is_optional<decltype(tmp1)>::value)
                    {
                        if (field.has_value())
                        {
                            using BaseType = typename remove_optional<decltype(tmp1)>::type;
                            if constexpr (is_struct_v<BaseType> && !std::is_same<BaseType, std::string>::value)
                            {
                                std::cout << idx << ": " << names[idx] << " = Optional" << ", value : " << std::endl;
                                BaseType value = field.value();
                                printObjectExt(value);
                            }
                            else
                            {
                                std::cout << idx << ": " << names[idx] << " = Optional" << ", value : ";
                                printField(std::cout, field.value()) << '\n';
                            }
                        }
                        else
                        {
                            std::cout << idx << ": " << names[idx] << " = " << "Optional: " << "not value!" << std::endl;
                        }
                    }
                    else if constexpr (std::is_pointer<decltype(tmp1)>::value)
                    {
                        using NonPointerType = std::remove_pointer_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonPointerType> && !std::is_same<NonPointerType, std::string>::value)
                        {
                            std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << std::endl;
                            NonPointerType value = *field;
                            printObjectExt(value);
                        }
                        else
                        {
                            if (field != nullptr)
                            {
                                std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << *field << std::endl;
                            }
                            else
                            {
                                std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << "nullptr" << std::endl;
                            }
                        }
                    }
                    else if constexpr (std::is_same<decltype(tmp1), std::string>::value)
                    {
                        std::cout << idx << ": " << names[idx] << " = ";
                        printField(std::cout, field) << '\n';
                    }

                    else if constexpr (is_struct_v<decltype(tmp1)>)
                    {
                        std::cout << idx << ": " << names[idx] << " = Class,Struct" << ", value : " << std::endl;
                        printObjectExt(field);
                    }
                    else
                    {
                        std::cout << idx << ": " << names[idx] << " = ";
                        printField(std::cout, field) << '\n';
                    }
                });
        }
    }

    template <typename T>
    void setJsonExt(T &obj, Json::Value &root, bool logging = false)
    {
        if constexpr (std::is_class_v<T> && !std::is_same_v<T, std::string>)
        {
            typename std::remove_reference<decltype(obj)>::type tmp;
            constexpr auto names = boost::pfr::names_as_array<decltype(tmp)>();
            // constexpr auto names = boost::pfr::names_as_array<T>();
            boost::pfr::for_each_field(
                obj,
                [&names, &root, logging](auto &field, std::size_t idx)
                {
                    auto nameField = std::string(names[idx]);
                    const std::string name = std::string(names[idx]);
                    typename std::remove_reference<decltype(field)>::type tmp1;
                    if constexpr (is_vector<decltype(tmp1)>::value)
                    {
                        auto size = field.size();
                        using NonVectorType = remove_vector_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonVectorType> && !std::is_same<NonVectorType, std::string>::value)
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << std::endl;
                            for (auto o : field)
                            {
                                if (logging)
                                    printObjectExt(o);

                                Json::Value temp;
                                setJsonExt(o, temp);
                                root[names[idx]] = temp;
                            }
                        }
                        else
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << "[";
                            int n = 0;
                            for (auto o : field)
                            {
                                if (logging)
                                    printField(std::cout, o);
                                root[names[idx]] = o;
                                n++;
                                if (logging)
                                {
                                    if (n != field.size())
                                    {
                                        std::cout << ",";
                                    }
                                }
                            }

                            if (logging)
                                std::cout << "]" << std::endl;
                        }
                    }
                    else if constexpr (is_optional<decltype(tmp1)>::value)
                    {
                        if (field.has_value())
                        {
                            using BaseType = typename remove_optional<decltype(tmp1)>::type;
                            if constexpr (is_struct_v<BaseType> && !std::is_same<BaseType, std::string>::value)
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = Optional" << ", value : " << std::endl;
                                BaseType value = field.value();
                                if (logging)
                                    printObjectExt(value);

                                Json::Value temp;
                                setJsonExt(value, temp);
                                root[names[idx]] = temp;
                            }
                            else
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = Optional" << ", value : ";
                                if (logging)
                                    printField(std::cout, field.value()) << '\n';
                                root[nameField] = field.value();
                            }
                        }
                        else
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << " = " << "Optional: " << "not value!" << std::endl;
                        }
                    }
                    else if constexpr (std::is_pointer<decltype(tmp1)>::value)
                    {
                        using NonPointerType = std::remove_pointer_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonPointerType> && !std::is_same<NonPointerType, std::string>::value)
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << std::endl;
                            NonPointerType value = *field;
                            if (logging)
                                printObjectExt(value);

                            Json::Value temp;
                            setJsonExt(value, temp);
                            root[nameField] = temp;
                        }
                        else
                        {
                            if (field != nullptr)
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << *field << std::endl;
                                root[nameField] = *field;
                            }
                            else
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << "nullptr" << std::endl;
                                root[nameField] = nullptr;
                            }
                        }
                    }
                    else if constexpr (std::is_same<decltype(tmp1), std::string>::value)
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = ";
                        if (logging)
                            printField(std::cout, field) << '\n';
                        // auto nameField = std::string(names[idx]);
                        root[nameField] = field;
                    }

                    else if constexpr (is_struct_v<decltype(tmp1)>)
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = Class,Struct" << ", value : " << std::endl;
                        if (logging)
                            printObjectExt(field);

                        Json::Value temp;
                        setJsonExt(field, temp);
                        root[nameField] = temp;
                    }
                    else
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = ";
                        if (logging)
                            printField(std::cout, field) << '\n';

                        root[nameField] = field;
                    }
                });
        }
    }

    template <typename T>
    Json::Value toJsonExt(T &obj, bool logging = false)
    {
        Json::Value root;
        if constexpr (is_vector<T>::value)
        {
            root["vector"] = "vector";
            // auto size = (std::vector<T>)obj.size();
            using NonVectorType = remove_vector_t<T>;
            if constexpr (is_struct_v<NonVectorType> && !std::is_same<NonVectorType, std::string>::value)
            {
                // if (logging)
                //     std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << std::endl;
                int n = 0;
                for (auto o : obj)
                {
                    // if (logging)
                    //     printObjectExt(o);

                    root[n] = toJsonExt(o);
                    n++;
                }
            }
            else
            {
                // if (logging)
                //     std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << "[";
                int n = 0;
                for (auto o : obj)
                {
                    if (logging)
                        printField(std::cout, o);

                    root.insert(o);

                    n++;
                    if (logging)
                    {
                        if (n != obj.size())
                        {
                            std::cout << ",";
                        }
                    }
                }

                if (logging)
                    std::cout << "]" << std::endl;
            }
        }
        else if constexpr (std::is_class_v<T> && !std::is_same_v<T, std::string>)
        {
            typename std::remove_reference<decltype(obj)>::type tmp;
            constexpr auto names = boost::pfr::names_as_array<decltype(tmp)>();
            // constexpr auto names = boost::pfr::names_as_array<T>();
            boost::pfr::for_each_field(
                obj,
                [&names, &root, logging](auto &field, std::size_t idx)
                {
                    auto nameField = std::string(names[idx]);
                    const std::string name = std::string(names[idx]);
                    typename std::remove_reference<decltype(field)>::type tmp1;
                    if constexpr (is_vector<decltype(tmp1)>::value)
                    {
                        auto size = field.size();
                        using NonVectorType = remove_vector_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonVectorType> && !std::is_same<NonVectorType, std::string>::value)
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << std::endl;
                            for (auto o : field)
                            {
                                if (logging)
                                    printObjectExt(o);

                                root[nameField] = toJsonExt(o);
                            }
                        }
                        else
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << ":" << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(field)>>().pretty_name() << ", size: " << size << ", " << " = " << "[";
                            int n = 0;
                            for (auto o : field)
                            {
                                if (logging)
                                    printField(std::cout, o);

                                root[nameField] = o;

                                n++;
                                if (logging)
                                {
                                    if (n != field.size())
                                    {
                                        std::cout << ",";
                                    }
                                }
                            }

                            if (logging)
                                std::cout << "]" << std::endl;
                        }
                    }
                    else if constexpr (is_optional<decltype(tmp1)>::value)
                    {
                        if (field.has_value())
                        {
                            using BaseType = typename remove_optional<decltype(tmp1)>::type;
                            if constexpr (is_struct_v<BaseType> && !std::is_same<BaseType, std::string>::value)
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = Optional" << ", value : " << std::endl;
                                BaseType value = field.value();
                                if (logging)
                                    printObjectExt(value);

                                root[nameField] = toJsonExt(value);
                            }
                            else
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = Optional" << ", value : ";
                                if (logging)
                                    printField(std::cout, field.value()) << '\n';
                                root[nameField] = field.value();
                            }
                        }
                        else
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << " = " << "Optional: " << "not value!" << std::endl;
                        }
                    }
                    else if constexpr (std::is_pointer<decltype(tmp1)>::value)
                    {
                        using NonPointerType = std::remove_pointer_t<decltype(tmp1)>;
                        if constexpr (is_struct_v<NonPointerType> && !std::is_same<NonPointerType, std::string>::value)
                        {
                            if (logging)
                                std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << std::endl;
                            NonPointerType value = *field;
                            if (logging)
                                printObjectExt(value);

                            root[nameField] = toJsonExt(value);
                        }
                        else
                        {
                            if (field != nullptr)
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << *field << std::endl;
                                root[nameField] = *field;
                            }
                            else
                            {
                                if (logging)
                                    std::cout << idx << ": " << names[idx] << " = " << field << ", value : " << "nullptr" << std::endl;
                                root[nameField] = nullptr;
                            }
                        }
                    }
                    else if constexpr (std::is_same<decltype(tmp1), std::string>::value)
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = ";
                        if (logging)
                            printField(std::cout, field) << '\n';
                        // auto nameField = std::string(names[idx]);
                        root[nameField] = field;
                    }
                    else if constexpr (std::is_same<decltype(tmp1), trantor::Date>::value)
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = ";
                        if (logging)
                            printField(std::cout, field.toDbStringLocal()) << '\n';
                        // auto nameField = std::string(names[idx]);
                        root[nameField] = field.toDbStringLocal();
                    }
                    else if constexpr (is_struct_v<decltype(tmp1)>)
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = Class,Struct" << ", value : " << std::endl;
                        if (logging)
                            printObjectExt(field);

                        root[nameField] = toJsonExt(field);
                    }
                    else
                    {
                        if (logging)
                            std::cout << idx << ": " << names[idx] << " = ";
                        if (logging)
                            printField(std::cout, field) << '\n';

                        root[nameField] = field;
                    }
                });
        }

        return root;
    }

    template <typename T>
    std::optional<int> findIndex(std::vector<T> arrays, std::function<bool(T)> func)
    {

        auto findedItem = std::find_if(arrays.begin(), arrays.end(), func);

        if (findedItem == arrays.end())
            return std::nullopt;

        return std::make_optional<int>(findedItem - arrays.begin());
    }

    template <typename T>
    std::optional<T> find(std::vector<T> arrays, std::function<bool(T)> func)
    {

        auto findedItem = std::find_if(arrays.begin(), arrays.end(), func);

        if (findedItem == arrays.end())
            return std::nullopt;

        return std::make_optional(*findedItem);
    }

    template <typename T>
    std::vector<T> filter(std::vector<T> arrays, std::function<bool(T)> func)
    {

        auto excludeItems = std::remove_if(arrays.begin(), arrays.end(), func);

        arrays.erase(excludeItems, arrays.end());

        return arrays;
    }

    inline std::string getNameFeild(const std::string &inputString, const std::string &excludeString)
    {

        // Tìm vị trí của ký tự "_" cuối cùng trong chuỗi
        std::size_t lastUnderscorePos = inputString.find_last_of(excludeString);

        std::string ret;
        if (lastUnderscorePos != std::string::npos)
        {
            // Tách phần chữ X lớn từ chuỗi bằng cách sử dụng substr từ vị trí của ký tự "_"
            ret = inputString.substr(0, lastUnderscorePos - (excludeString.length()));
        }

        return ret;
    }

    inline std::string setNameFeild(const std::string &inputString, const std::string &insertString)
    {

        return inputString + "_" + insertString;
    }

    template <typename T>
    std::string to_string_generic(const T &value)
    {
        // Kiểm tra nếu là std::string
        if constexpr (std::is_same_v<T, std::string>)
        {
            return value;
        }
        // // Kiểm tra nếu là std::vector hoặc kiểu phức tạp khác (không hỗ trợ)
        // else if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
        // {
        //     // Xử lý riêng cho vector hoặc các container, trả về chuỗi rỗng
        //     return "";
        // }
        // Kiểm tra nếu là std::vector
        else if constexpr (is_std_vector<T>::value)
        {
            // Xử lý riêng cho vector, trả về chuỗi rỗng
            return "";
        }
        // Nếu là kiểu cơ bản, dùng std::to_string
        else if constexpr (std::is_arithmetic_v<T>)
        {
            return std::to_string(value);
        }
        // Nếu không hỗ trợ kiểu khác, trả về chuỗi rỗng
        else
        {
            return "";
        }
    }

    template <typename TypeB, typename TypeParent>
    std::vector<TypeB> groupJoin(Json::Value j, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix = std::string{""}, std::string parent = std::string{""});

    template <typename TypeB, typename TypeParent>
    TypeB groupRowOneToMany(Json::Value row, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix = std::string{""}, std::string parent = std::string{""})
    {
        if constexpr (!is_struct_v<TypeB>)
        {
            return row.template as<TypeB>();
        }
        // if (!row.isObject())
        // {

        //     // static_assert(std::is_same_v<TypeB, decltype(row)>, "Value is not an int");

        //     return (TypeB)row;
        // }
        else
        {
            TypeB objB;
            constexpr auto names = boost::pfr::names_as_array<TypeB>();
            boost::pfr::for_each_field(
                objB,
                [&](auto &fieldB, std::size_t idx)
                {
                    const std::string name = std::string(names[idx]);
                    typename std::remove_reference<decltype(fieldB)>::type tmp;

                    // tại sao cho constexpr  và  if constexpr (std::is_same<decltype(tmp), A>::value) lại compile được?
                    // Khi sử dụng if constexpr, bạn đang tận dụng tính năng của C++17 cho phép bạn thực hiện việc "đánh giá" các điều kiện tại thời gian biên dịch (compile-time).
                    // Điều này có nghĩa là chỉ những nhánh của if constexpr được đánh giá là đúng mới được biên dịch và thực thi, còn những nhánh sai sẽ bị loại bỏ hoàn toàn.

                    // Vì sao constexpr và if constexpr cho phép biên dịch thành công?
                    // constexpr: Đảm bảo rằng giá trị hoặc hàm được đánh giá trong thời gian biên dịch, giúp tối ưu hóa mã và loại bỏ các lỗi có thể phát sinh trong thời gian chạy (run-time).

                    // if constexpr: Dựa vào điều kiện, nếu biểu thức bên trong if constexpr là đúng (true), đoạn mã tương ứng sẽ được biên dịch. Nếu biểu thức là sai (false), đoạn mã sẽ bị bỏ qua.

                    // Cụ thể, trong trường hợp if constexpr (std::is_same<decltype(tmp), A>::value), nếu decltype(tmp) là A, điều kiện sẽ là true và đoạn mã bên trong sẽ được biên dịch.
                    // Nếu không, đoạn mã đó sẽ bị bỏ qua, tránh lỗi biên dịch liên quan đến việc gán một đối tượng A cho kiểu dữ liệu khác như int.

                    // Tóm lại:
                    // constexpr: Đảm bảo các hằng số và các phép tính có thể được xử lý tại thời gian biên dịch.
                    // if constexpr: Đảm bảo rằng chỉ những đoạn mã có điều kiện đúng mới được biên dịch, giúp tránh các lỗi không cần thiết.
                    if constexpr (is_vector<decltype(tmp)>::value)
                    {
                        remove_vector_t<decltype(tmp)> elementType;
                        const auto nameTypeB = std::string(get_type<decltype(elementType)>());
                        auto primaryKey = getPrimaryKey<decltype(elementType)>();

                        std::string prefixName = nameTypeB;
                        if (prefix != "")
                        {
                            prefixName = prefix + "." + nameTypeB;
                        }

                        decltype(elementType) objTemp;
                        constexpr auto namesFieldTemp = boost::pfr::names_as_array<decltype(elementType)>();
                        // std::string parent = std::string{};

                        // cal hash for resultMap
                        boost::pfr::for_each_field(
                            objTemp,
                            [&](auto &fieldCheck, std::size_t idxCheck)
                            {
                                std::string nameFix = std::string(namesFieldTemp[idxCheck]);
                                auto temp = fieldCheck;
                                if (nameFix == primaryKey)
                                {
                                    auto prefixName = primaryKey;
                                    if (prefix != "")
                                    {
                                        prefixName = prefix + "." + primaryKey;
                                    }
                                    if constexpr (std::is_arithmetic_v<decltype(temp)>)
                                    {
                                        if (!row[prefixName].isNull())
                                        {
                                            fieldCheck = row[prefixName].template as<decltype(temp)>();
                                            parent = parent + prefix + to_string_generic(fieldCheck);
                                        }
                                        else
                                        {
                                            fieldCheck = DefaultValue<decltype(temp)>::value();
                                        }
                                    }
                                }
                            });

                        // auto value = row[name];
                        // std::cout << "groupRowOneToMany; TypeB: " << get_type<decltype(elementType)>() << " TypeParent: " << get_type<TypeB>() << std::endl;
                        decltype(tmp) *pointer = new decltype(tmp);
                        *pointer = groupJoin<decltype(elementType), TypeB>(row, resultMap, typesToCheck, prefixName, parent);
                        fieldB = *pointer;
                        // resultMap[prefixName + fieldB.id] = prefixName + fieldB.id;

                        decltype(elementType) *elementFirst = new decltype(elementType);
                        *elementFirst = (*pointer)[0];

                        constexpr auto namesField = boost::pfr::names_as_array<decltype(elementType)>();
                        boost::pfr::for_each_field(
                            *elementFirst,
                            [&](auto &fieldCheck, std::size_t idxCheck)
                            {
                                std::string nameFix = std::string(namesField[idxCheck]);
                                if (nameFix == primaryKey)
                                {
                                    // resultMap[parent + prefixName + to_string_generic(fieldCheck)] = parent + prefixName + to_string_generic(fieldCheck);

                                    // std::cout << "groupRowOneToMany; TypeB: " << get_type<decltype(elementType)>() << " TypeParent: " << get_type<TypeB>() << std::endl;

                                    std::string hashItem = parent + prefixName + to_string_generic(fieldCheck);
                                    if (resultMap.find(hashItem) != resultMap.end())
                                    {
                                    }
                                    else
                                    {
                                        resultMap[parent + prefixName + to_string_generic(fieldCheck)] = elementFirst;
                                    }

                                    if (resultMap.find(parent) != resultMap.end())
                                    {
                                        auto value = resultMap[parent];
                                        auto nameParent = get_type<TypeB>();
                                        constexpr auto namesFieldCast = boost::pfr::names_as_array<TypeB>();
                                        if (value.type() == typeid(TypeB *))
                                        {
                                            auto valueCast = std::any_cast<TypeB *>(value);

                                            boost::pfr::for_each_field(
                                                *valueCast,
                                                [&](auto &fieldCast, std::size_t idxCast)
                                                {
                                                    std::string nameCastFix = std::string(namesFieldCast[idxCast]);
                                                    typename std::remove_reference<decltype(fieldCast)>::type tmpCast;
                                                    if (nameCastFix.compare(name) == 0)
                                                    {
                                                        if constexpr (is_vector<decltype(tmpCast)>::value)
                                                        {
                                                            // fieldCast.push_back(*elementFirst);
                                                            if constexpr (std::is_same<decltype(tmpCast), decltype(tmp)>::value)
                                                            {
                                                                // std::cout << "Same: nameParent: " << get_type<TypeB>() << ", " << "ParentHash: " << parent << ", " << get_type<decltype(tmpCast)>() << std::endl;

                                                                // Lấy thời gian hiện tại
                                                                auto start = std::chrono::high_resolution_clock::now();

                                                                int index = 0;
                                                                bool flagAdd = true;
                                                                for (auto item : fieldCast)
                                                                {
                                                                    // std::cout << "itemhash: " << getItemHash<decltype(elementType)>(item, parent, prefixName) << std::endl;
                                                                    if (hashItem == getItemHash<decltype(elementType)>(item, parent, prefixName))
                                                                    {
                                                                        auto valueItem = std::any_cast<decltype(elementType) *>(resultMap[hashItem]);

                                                                        fieldCast[index] = *valueItem;
                                                                        flagAdd = false;
                                                                    }
                                                                    index++;
                                                                }

                                                                // Lấy thời gian sau khi hoàn thành công việc
                                                                auto end = std::chrono::high_resolution_clock::now();

                                                                // Tính thời gian thực thi (khoảng thời gian)
                                                                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                                                                if (resultMap.find("__TimeForLoop__") != resultMap.end())
                                                                {
                                                                    auto durationTemp = std::any_cast<std::chrono::microseconds>(resultMap["__TimeForLoop__"]);
                                                                    resultMap["__TimeForLoop__"] = durationTemp + duration;
                                                                }
                                                                else
                                                                {
                                                                    resultMap["__TimeForLoop__"] = duration;
                                                                };

                                                                if (flagAdd)
                                                                    fieldCast.push_back(*elementFirst);
                                                                // fieldCast.push_back(*elementFirst);
                                                            }
                                                        }
                                                    }
                                                });

                                            // std::cout
                                            //     << valueCast << std::endl;
                                        }
                                    }
                                }
                            });
                        // std::cout
                        //     << name << ", " << get_type<decltype(fieldB)>() << std::endl;
                    }
                    else if constexpr (is_optional<decltype(tmp)>::value)
                    {
                        const auto nameTypeB = std::string(get_type<TypeB>());
                        using BaseType = typename remove_optional<decltype(tmp)>::type;

                        if constexpr (is_struct_v<BaseType> && !std::is_same<BaseType, std::string>::value)
                        {
                            // BaseType value;
                            // assignFields(value, row, get_type<BaseType>());
                            fieldB = groupRowOneToMany<BaseType>(row, resultMap, typesToCheck);
                            std::cout
                                << name << ", " << "Optional: " << get_type<decltype(fieldB)>() << std::endl;
                        }
                        else
                        {
                            if (!row[name].isNull())
                            {
                                BaseType value = row[name].template as<BaseType>();
                                fieldB = value;
                                std::cout << name << ", " << "Optional: " << get_type<decltype(fieldB)>() << std::endl;
                            }
                            else
                            {
                                fieldB = std::nullopt;
                            }
                        }
                    }

                    else if constexpr (is_struct_v<decltype(tmp)> && !std::is_same<decltype(tmp), std::string>::value)
                    {
                        fieldB = groupRowOneToMany<decltype(tmp)>(row, resultMap, typesToCheck);
                        std::cout
                            << name << ", " << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(fieldB)>>().pretty_name() << std::endl;
                        // objAs.push_back(fieldB);
                    }
                    else if constexpr (std::is_pointer<decltype(tmp)>::value)
                    {
                        using NonPointerType = std::remove_pointer_t<decltype(tmp)>;
                        const auto nameTypeB = std::string(get_type<TypeB>());

                        if constexpr (is_struct_v<NonPointerType> && !std::is_same<NonPointerType, std::string>::value)
                        {
                            // Không dùng NonPointerType value --> nên dùng con trỏ, nếu không sẽ bị lỗi, do value cùng địa chỉ cho tất cả các row!!!!!!
                            NonPointerType *value = new NonPointerType();
                            // assignFields(*value, row, get_type<NonPointerType>()); // Gọi đệ quy để xử lý các trường của a
                            *value = groupRowOneToMany<NonPointerType>(row, resultMap, typesToCheck);
                            fieldB = value;
                            std::cout
                                << name << ", " << "Poiter: " << get_type<decltype(fieldB)>() << std::endl;
                            // printObjectExt(value);
                        }
                        else
                        {
                            if (!row[name].isNull())
                            {
                                NonPointerType value = row[name].template as<NonPointerType>();
                                fieldB = new NonPointerType(value); // Tạo giá trị mới và gán địa chỉ cho con trỏ
                            }
                            else
                            {
                                fieldB = nullptr;
                            }
                            std::cout << name << ", " << "Poiter: " << get_type<decltype(fieldB)>() << std::endl;
                        }
                    }

                    else
                    {
                        // field = row[name].get<typename std::remove_reference<decltype(field)>::type>();
                        // typename std::remove_reference<decltype(fieldB)>::type tmp;
                        auto temp = fieldB;

                        const auto nameTypeB = std::string(get_type<TypeB>());
                        std::string prefixName;
                        prefixName = name;
                        if (prefix != "")
                        {
                            prefixName = prefix + "." + name;
                        }
                        if (!row[prefixName].isNull())
                        {
                            fieldB = row[prefixName].template as<decltype(temp)>();
                        }
                        else
                        {
                            fieldB = DefaultValue<decltype(temp)>::value();
                        }

                        // row[setNameFeild(name, "B")].get<decltype(temp)>();

                        // std::decay_t sẽ loại bỏ &, &&, và const, giúp bạn hiển thị tên loại gốc của biến mà không có các ký hiệu này
                        // Để hiển thị kiểu dữ liệu rõ ràng hơn, bạn có thể sử dụng thư viện như boost::typeindex
                        // std::cout
                        //     << name << ", " << get_type<decltype(fieldB)>() << std::endl;
                    }
                });
            return objB;
            // std::cout << typeid(row["id_A"]).name() << std::endl;
        }
    }

    template <typename TypeB, typename TypeParent>
    std::vector<TypeB> groupJoin(Json::Value j, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix, std::string parent)
    {

        std::vector<TypeB> objBs;

        // lưu hash trong map cua item return
        std::vector<std::string> topHash;
        // lưu item return
        std::vector<TypeB *> itemReturns;

        // std::map<std::string, std::any> resultMap;
        // std::cout << "TypeB: " << get_type<TypeB>() << " TypeParent: " << get_type<TypeParent>() << std::endl;
        // std::cout << "TypeB process: " << get_type<TypeB>() << " ,is array: " << j.is_array() << " ,size: " << j.size() << " ,value: " << j.dump() << std::endl;
        if (j.isArray())
        {
            for (const auto &row : j)
            {
                auto primaryKey = getPrimaryKey<TypeB>();

                TypeB objTemp;

                // Lỗi này xảy ra vì bạn đã khai báo một std::map<std::string, std::type_index>
                // nhưng std::type_index không có constructor mặc định. std::type_index yêu cầu một kiểu hợp lệ để khởi tạo.
                // Khi bạn khởi tạo một phần tử mới trong std::map với cú pháp typesToCheck["type"],
                // điều đó buộc map phải khởi tạo giá trị mặc định cho std::type_index.
                // Vì std::type_index không có constructor mặc định, nên bạn sẽ gặp lỗi biên dịch.
                // hiện không cần dùng đến typesToCheck
                typesToCheck.insert({get_type<TypeB>(), typeid(objTemp)});

                constexpr auto namesFieldTemp = boost::pfr::names_as_array<TypeB>();
                // std::string parent = std::string{};
                // cal hash for resultMap
                boost::pfr::for_each_field(
                    objTemp,
                    [&](auto &fieldCheck, std::size_t idxCheck)
                    {
                        std::string nameFix = std::string(namesFieldTemp[idxCheck]);
                        auto temp = fieldCheck;
                        if (nameFix == primaryKey)
                        {
                            auto prefixName = primaryKey;
                            if (prefix != "")
                            {
                                prefixName = prefix + "." + primaryKey;
                            }
                            if constexpr (std::is_arithmetic_v<decltype(temp)>)
                            {
                                if (!row[prefixName].is_null())
                                {
                                    fieldCheck = row[prefixName].template get<decltype(temp)>();
                                    parent = parent + prefix + to_string_generic(fieldCheck);
                                }
                                else
                                {
                                    fieldCheck = DefaultValue<decltype(temp)>::value();
                                }
                            }
                        }
                    });

                TypeB *obj = new TypeB();
                *obj = groupRowOneToMany<TypeB, TypeParent>(row, resultMap, typesToCheck, prefix, parent);

                constexpr auto namesField = boost::pfr::names_as_array<TypeB>();

                std::string hashItem = std::string{""};
                boost::pfr::for_each_field(
                    *obj,
                    [&](auto &fieldCheck, std::size_t idxCheck)
                    {
                        std::string nameFix = std::string(namesField[idxCheck]);
                        if (nameFix == primaryKey)
                        {
                            hashItem = to_string_generic(fieldCheck);
                        }
                    });
                // resultMap[prefix + to_string_generic(fieldCheck)] = prefix + to_string_generic(fieldCheck);

                if (resultMap.find(parent) != resultMap.end())
                {
                }
                else
                {
                    resultMap[prefix + hashItem] = obj;
                    itemReturns.push_back(obj);
                    topHash.push_back(prefix + hashItem);
                }

                objBs.push_back(*obj);

                parent = "";
            }
        }
        else if (j.isObject())
        {
            TypeB objTemp;
            typesToCheck.insert({get_type<TypeB>(), typeid(objTemp)});
            objBs.push_back(groupRowOneToMany<TypeB, TypeParent>(j, resultMap, typesToCheck, prefix, parent));
        }

        resultMap["__topHash__"] = topHash;
        resultMap["__itemReturns__"] = itemReturns;
        return objBs;
    }

    template <typename TypeB, typename TypeParent>
    std::vector<TypeB> groupJoinDrogon(Result j, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix = std::string{""}, std::string parent = std::string{""});

    template <typename TypeB, typename TypeParent, typename = std::enable_if_t<std::is_class<TypeB>::value>>
    TypeB groupRowOneToManyDrogon(Row row, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix = std::string{""}, std::string parent = std::string{""})
    {
        std::map<std::string, int> mapRow = {};

        for (int i = 0; i < row.size(); i++)
        {
            mapRow[row[i].name()] = i;
        }

        TypeB objB;

        constexpr auto names = boost::pfr::names_as_array<TypeB>();
        boost::pfr::for_each_field(
            objB,
            [&](auto &fieldB, std::size_t idx)
            {
                const std::string name = std::string(names[idx]);
                typename std::remove_reference<decltype(fieldB)>::type tmp;

                // tại sao cho constexpr  và  if constexpr (std::is_same<decltype(tmp), A>::value) lại compile được?
                // Khi sử dụng if constexpr, bạn đang tận dụng tính năng của C++17 cho phép bạn thực hiện việc "đánh giá" các điều kiện tại thời gian biên dịch (compile-time).
                // Điều này có nghĩa là chỉ những nhánh của if constexpr được đánh giá là đúng mới được biên dịch và thực thi, còn những nhánh sai sẽ bị loại bỏ hoàn toàn.

                // Vì sao constexpr và if constexpr cho phép biên dịch thành công?
                // constexpr: Đảm bảo rằng giá trị hoặc hàm được đánh giá trong thời gian biên dịch, giúp tối ưu hóa mã và loại bỏ các lỗi có thể phát sinh trong thời gian chạy (run-time).

                // if constexpr: Dựa vào điều kiện, nếu biểu thức bên trong if constexpr là đúng (true), đoạn mã tương ứng sẽ được biên dịch. Nếu biểu thức là sai (false), đoạn mã sẽ bị bỏ qua.

                // Cụ thể, trong trường hợp if constexpr (std::is_same<decltype(tmp), A>::value), nếu decltype(tmp) là A, điều kiện sẽ là true và đoạn mã bên trong sẽ được biên dịch.
                // Nếu không, đoạn mã đó sẽ bị bỏ qua, tránh lỗi biên dịch liên quan đến việc gán một đối tượng A cho kiểu dữ liệu khác như int.

                // Tóm lại:
                // constexpr: Đảm bảo các hằng số và các phép tính có thể được xử lý tại thời gian biên dịch.
                // if constexpr: Đảm bảo rằng chỉ những đoạn mã có điều kiện đúng mới được biên dịch, giúp tránh các lỗi không cần thiết.
                if constexpr (is_vector<decltype(tmp)>::value)
                {
                    remove_vector_t<decltype(tmp)> elementType;
                    const auto nameTypeB = std::string(get_type<decltype(elementType)>());
                    auto primaryKey = getPrimaryKey<decltype(elementType)>();

                    std::string prefixName = nameTypeB;
                    if (prefix != "")
                    {
                        prefixName = prefix + "." + nameTypeB;
                    }

                    decltype(elementType) objTemp;
                    constexpr auto namesFieldTemp = boost::pfr::names_as_array<decltype(elementType)>();
                    // std::string parent = std::string{};

                    // cal hash for resultMap
                    boost::pfr::for_each_field(
                        objTemp,
                        [&](auto &fieldCheck, std::size_t idxCheck)
                        {
                            std::string nameFix = std::string(namesFieldTemp[idxCheck]);
                            auto temp = fieldCheck;
                            if (nameFix == primaryKey)
                            {
                                auto prefixName = primaryKey;
                                if (prefix != "")
                                {
                                    prefixName = prefix + "." + primaryKey;
                                }
                                if constexpr (std::is_arithmetic_v<decltype(temp)>)
                                {
                                    if (!row[mapRow[prefixName]].isNull())
                                    {
                                        fieldCheck = row[mapRow[prefixName]].template as<decltype(temp)>();
                                        parent = parent + prefix + to_string_generic(fieldCheck);
                                    }
                                    else
                                    {
                                        fieldCheck = DefaultValue<decltype(temp)>::value();
                                    }
                                }
                            }
                        });

                    // auto value = row[name];
                    // std::cout << "groupRowOneToMany; TypeB: " << get_type<decltype(elementType)>() << " TypeParent: " << get_type<TypeB>() << std::endl;
                    decltype(tmp) *pointer = new decltype(tmp);
                    *pointer = groupJoin<decltype(elementType), TypeB>(row, resultMap, typesToCheck, prefixName, parent);
                    fieldB = *pointer;
                    // resultMap[prefixName + fieldB.id] = prefixName + fieldB.id;

                    decltype(elementType) *elementFirst = new decltype(elementType);
                    *elementFirst = (*pointer)[0];

                    constexpr auto namesField = boost::pfr::names_as_array<decltype(elementType)>();
                    boost::pfr::for_each_field(
                        *elementFirst,
                        [&](auto &fieldCheck, std::size_t idxCheck)
                        {
                            std::string nameFix = std::string(namesField[idxCheck]);
                            if (nameFix == primaryKey)
                            {
                                // resultMap[parent + prefixName + to_string_generic(fieldCheck)] = parent + prefixName + to_string_generic(fieldCheck);

                                // std::cout << "groupRowOneToMany; TypeB: " << get_type<decltype(elementType)>() << " TypeParent: " << get_type<TypeB>() << std::endl;

                                std::string hashItem = parent + prefixName + to_string_generic(fieldCheck);
                                if (resultMap.find(hashItem) != resultMap.end())
                                {
                                }
                                else
                                {
                                    resultMap[parent + prefixName + to_string_generic(fieldCheck)] = elementFirst;
                                }

                                if (resultMap.find(parent) != resultMap.end())
                                {
                                    auto value = resultMap[parent];
                                    auto nameParent = get_type<TypeB>();
                                    constexpr auto namesFieldCast = boost::pfr::names_as_array<TypeB>();
                                    if (value.type() == typeid(TypeB *))
                                    {
                                        auto valueCast = std::any_cast<TypeB *>(value);

                                        boost::pfr::for_each_field(
                                            *valueCast,
                                            [&](auto &fieldCast, std::size_t idxCast)
                                            {
                                                std::string nameCastFix = std::string(namesFieldCast[idxCast]);
                                                typename std::remove_reference<decltype(fieldCast)>::type tmpCast;
                                                if (nameCastFix.compare(name) == 0)
                                                {
                                                    if constexpr (is_vector<decltype(tmpCast)>::value)
                                                    {
                                                        // fieldCast.push_back(*elementFirst);
                                                        if constexpr (std::is_same<decltype(tmpCast), decltype(tmp)>::value)
                                                        {
                                                            // std::cout << "Same: nameParent: " << get_type<TypeB>() << ", " << "ParentHash: " << parent << ", " << get_type<decltype(tmpCast)>() << std::endl;

                                                            // Lấy thời gian hiện tại
                                                            auto start = std::chrono::high_resolution_clock::now();

                                                            int index = 0;
                                                            bool flagAdd = true;
                                                            for (auto item : fieldCast)
                                                            {
                                                                // std::cout << "itemhash: " << getItemHash<decltype(elementType)>(item, parent, prefixName) << std::endl;
                                                                if (hashItem == getItemHash<decltype(elementType)>(item, parent, prefixName))
                                                                {
                                                                    auto valueItem = std::any_cast<decltype(elementType) *>(resultMap[hashItem]);

                                                                    fieldCast[index] = *valueItem;
                                                                    flagAdd = false;
                                                                }
                                                                index++;
                                                            }

                                                            // Lấy thời gian sau khi hoàn thành công việc
                                                            auto end = std::chrono::high_resolution_clock::now();

                                                            // Tính thời gian thực thi (khoảng thời gian)
                                                            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                                                            if (resultMap.find("__TimeForLoop__") != resultMap.end())
                                                            {
                                                                auto durationTemp = std::any_cast<std::chrono::microseconds>(resultMap["__TimeForLoop__"]);
                                                                resultMap["__TimeForLoop__"] = durationTemp + duration;
                                                            }
                                                            else
                                                            {
                                                                resultMap["__TimeForLoop__"] = duration;
                                                            };

                                                            if (flagAdd)
                                                                fieldCast.push_back(*elementFirst);
                                                            // fieldCast.push_back(*elementFirst);
                                                        }
                                                    }
                                                }
                                            });

                                        // std::cout
                                        //     << valueCast << std::endl;
                                    }
                                }
                            }
                        });
                    // std::cout
                    //     << name << ", " << get_type<decltype(fieldB)>() << std::endl;
                }
                else if constexpr (is_optional<decltype(tmp)>::value)
                {
                    const auto nameTypeB = std::string(get_type<TypeB>());
                    using BaseType = typename remove_optional<decltype(tmp)>::type;

                    if constexpr (is_struct_v<BaseType> && !std::is_same<BaseType, std::string>::value)
                    {
                        // BaseType value;
                        // assignFields(value, row, get_type<BaseType>());
                        fieldB = groupRowOneToMany<BaseType>(row, resultMap, typesToCheck);
                        std::cout
                            << name << ", " << "Optional: " << get_type<decltype(fieldB)>() << std::endl;
                    }
                    else
                    {
                        if (!row[mapRow[name]].isNull())
                        {
                            BaseType value = row[mapRow[name]].template as<BaseType>();
                            fieldB = value;
                            std::cout << name << ", " << "Optional: " << get_type<decltype(fieldB)>() << std::endl;
                        }
                        else
                        {
                            fieldB = std::nullopt;
                        }
                    }
                }
                else if constexpr (is_struct_v<decltype(tmp)> && std::is_same<decltype(tmp), trantor::Date>::value)
                {
                    auto timeStr = row[mapRow[name]].as<std::string>();
                    struct tm stm;
                    memset(&stm, 0, sizeof(stm));
                    auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
                    time_t t = mktime(&stm);
                    size_t decimalNum = 0;
                    if (p)
                    {
                        if (*p == '.')
                        {
                            std::string decimals(p + 1, &timeStr[timeStr.length()]);
                            while (decimals.length() < 6)
                            {
                                decimals += "0";
                            }
                            decimalNum = (size_t)atol(decimals.c_str());
                        }
                        fieldB = trantor::Date(t * 1000000 + decimalNum);
                    }
                }
                else if constexpr (is_struct_v<decltype(tmp)> && !std::is_same<decltype(tmp), std::string>::value)
                {
                    fieldB = groupRowOneToManyDrogon<decltype(tmp)>(row, resultMap, typesToCheck);
                    std::cout
                        << name << ", " << boost::typeindex::type_id_with_cvr<std::decay_t<decltype(fieldB)>>().pretty_name() << std::endl;
                    // objAs.push_back(fieldB);
                }
                else if constexpr (std::is_pointer<decltype(tmp)>::value)
                {
                    using NonPointerType = std::remove_pointer_t<decltype(tmp)>;
                    const auto nameTypeB = std::string(get_type<TypeB>());

                    if constexpr (is_struct_v<NonPointerType> && !std::is_same<NonPointerType, std::string>::value)
                    {
                        // Không dùng NonPointerType value --> nên dùng con trỏ, nếu không sẽ bị lỗi, do value cùng địa chỉ cho tất cả các row!!!!!!
                        NonPointerType *value = new NonPointerType();
                        // assignFields(*value, row, get_type<NonPointerType>()); // Gọi đệ quy để xử lý các trường của a
                        *value = groupRowOneToMany<NonPointerType>(row, resultMap, typesToCheck);
                        fieldB = value;
                        std::cout
                            << name << ", " << "Poiter: " << get_type<decltype(fieldB)>() << std::endl;
                        // printObjectExt(value);
                    }
                    else
                    {
                        if (!row[mapRow[name]].isNull())
                        {
                            NonPointerType value = row[mapRow[name]].template as<NonPointerType>();
                            fieldB = new NonPointerType(value); // Tạo giá trị mới và gán địa chỉ cho con trỏ
                        }
                        else
                        {
                            fieldB = nullptr;
                        }
                        std::cout << name << ", " << "Poiter: " << get_type<decltype(fieldB)>() << std::endl;
                    }
                }

                else
                {
                    // field = row[name].get<typename std::remove_reference<decltype(field)>::type>();
                    // typename std::remove_reference<decltype(fieldB)>::type tmp;
                    auto temp = fieldB;

                    const auto nameTypeB = std::string(get_type<TypeB>());
                    std::string prefixName;
                    prefixName = name;
                    if (prefix != "")
                    {
                        prefixName = prefix + "." + name;
                    }
                    if (!row[mapRow[prefixName]].isNull())
                    {
                        fieldB = row[mapRow[prefixName]].template as<decltype(temp)>();
                    }
                    else
                    {
                        fieldB = DefaultValue<decltype(temp)>::value();
                    }

                    // row[setNameFeild(name, "B")].get<decltype(temp)>();

                    // std::decay_t sẽ loại bỏ &, &&, và const, giúp bạn hiển thị tên loại gốc của biến mà không có các ký hiệu này
                    // Để hiển thị kiểu dữ liệu rõ ràng hơn, bạn có thể sử dụng thư viện như boost::typeindex
                    // std::cout
                    //     << name << ", " << get_type<decltype(fieldB)>() << std::endl;
                }
            });
        return objB;
        // std::cout << typeid(row["id_A"]).name() << std::endl;
    }

    template <typename TypeB, typename TypeParent>
    std::vector<TypeB> groupJoinDrogon(Result j, std::map<std::string, std::any> &resultMap, std::map<std::string, std::type_index> &typesToCheck, std::string prefix, std::string parent)
    {

        std::vector<TypeB> objBs;

        // lưu hash trong map cua item return
        std::vector<std::string> topHash;
        // lưu item return
        std::vector<TypeB *> itemReturns;

        // std::map<std::string, std::any> resultMap;
        // std::cout << "TypeB: " << get_type<TypeB>() << " TypeParent: " << get_type<TypeParent>() << std::endl;
        // std::cout << "TypeB process: " << get_type<TypeB>() << " ,is array: " << j.is_array() << " ,size: " << j.size() << " ,value: " << j.dump() << std::endl;
        for (const auto &row : j)
        {
            std::map<std::string, int> mapRow = {};

            for (int i = 0; i < row.size(); i++)
            {
                mapRow[row[i].name()] = i;
            }

            auto primaryKey = getPrimaryKey<TypeB>();

            TypeB objTemp;

            // Lỗi này xảy ra vì bạn đã khai báo một std::map<std::string, std::type_index>
            // nhưng std::type_index không có constructor mặc định. std::type_index yêu cầu một kiểu hợp lệ để khởi tạo.
            // Khi bạn khởi tạo một phần tử mới trong std::map với cú pháp typesToCheck["type"],
            // điều đó buộc map phải khởi tạo giá trị mặc định cho std::type_index.
            // Vì std::type_index không có constructor mặc định, nên bạn sẽ gặp lỗi biên dịch.
            // hiện không cần dùng đến typesToCheck
            typesToCheck.insert({get_type<TypeB>(), typeid(objTemp)});

            constexpr auto namesFieldTemp = boost::pfr::names_as_array<TypeB>();
            // std::string parent = std::string{};
            // cal hash for resultMap
            boost::pfr::for_each_field(
                objTemp,
                [&](auto &fieldCheck, std::size_t idxCheck)
                {
                    std::string nameFix = std::string(namesFieldTemp[idxCheck]);
                    auto temp = fieldCheck;
                    if (nameFix == primaryKey)
                    {
                        auto prefixName = primaryKey;
                        if (prefix != "")
                        {
                            prefixName = prefix + "." + primaryKey;
                        }
                        if constexpr (std::is_arithmetic_v<decltype(temp)>)
                        {
                            if (!row[mapRow[prefixName]].isNull())
                            {
                                fieldCheck = row[mapRow[prefixName]].template as<decltype(temp)>();
                                parent = parent + prefix + to_string_generic(fieldCheck);
                            }
                            else
                            {
                                fieldCheck = DefaultValue<decltype(temp)>::value();
                            }
                        }
                    }
                });

            TypeB *obj = new TypeB();
            *obj = groupRowOneToManyDrogon<TypeB, TypeParent>(row, resultMap, typesToCheck, prefix, parent);

            constexpr auto namesField = boost::pfr::names_as_array<TypeB>();

            std::string hashItem = std::string{""};
            boost::pfr::for_each_field(
                *obj,
                [&](auto &fieldCheck, std::size_t idxCheck)
                {
                    std::string nameFix = std::string(namesField[idxCheck]);
                    if (nameFix == primaryKey)
                    {
                        hashItem = to_string_generic(fieldCheck);
                    }
                });
            // resultMap[prefix + to_string_generic(fieldCheck)] = prefix + to_string_generic(fieldCheck);

            if (resultMap.find(parent) != resultMap.end())
            {
            }
            else
            {
                resultMap[prefix + hashItem] = obj;
                itemReturns.push_back(obj);
                topHash.push_back(prefix + hashItem);
            }

            objBs.push_back(*obj);

            parent = "";
        }

        resultMap["__topHash__"] = topHash;
        resultMap["__itemReturns__"] = itemReturns;
        return objBs;
    }

}

#endif
