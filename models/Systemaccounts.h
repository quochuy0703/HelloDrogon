/**
 *
 *  Systemaccounts.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/BaseBuilder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace test
{

class Systemaccounts
{
  public:
    struct Cols
    {
        static const std::string _accountid;
        static const std::string _username;
        static const std::string _passwordhash;
        static const std::string _securitystamp;
        static const std::string _displayname;
        static const std::string _avatar;
        static const std::string _type;
        static const std::string _accessfailedcount;
        static const std::string _email;
        static const std::string _isemailconfirmed;
        static const std::string _phonenumber;
        static const std::string _isphonenumberconfirmed;
        static const std::string _twofactorenabled;
        static const std::string _sessionid;
        static const std::string _status;
        static const std::string _privatekey;
        static const std::string _isfirsttimelogin;
        static const std::string _ishrreference;
        static const std::string _session;
        static const std::string _createddate;
        static const std::string _createdby;
        static const std::string _updateddate;
        static const std::string _updatedby;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = void;
    int getPrimaryKey() const { assert(false); return 0; }

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Systemaccounts(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Systemaccounts(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Systemaccounts(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Systemaccounts() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    /**  For column accountid  */
    ///Get the value of the column accountid, returns the default value if the column is null
    const std::string &getValueOfAccountid() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getAccountid() const noexcept;
    ///Set the value of the column accountid
    void setAccountid(const std::string &pAccountid) noexcept;
    void setAccountid(std::string &&pAccountid) noexcept;
    void setAccountidToNull() noexcept;

    /**  For column username  */
    ///Get the value of the column username, returns the default value if the column is null
    const std::string &getValueOfUsername() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getUsername() const noexcept;
    ///Set the value of the column username
    void setUsername(const std::string &pUsername) noexcept;
    void setUsername(std::string &&pUsername) noexcept;
    void setUsernameToNull() noexcept;

    /**  For column passwordhash  */
    ///Get the value of the column passwordhash, returns the default value if the column is null
    const std::string &getValueOfPasswordhash() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPasswordhash() const noexcept;
    ///Set the value of the column passwordhash
    void setPasswordhash(const std::string &pPasswordhash) noexcept;
    void setPasswordhash(std::string &&pPasswordhash) noexcept;
    void setPasswordhashToNull() noexcept;

    /**  For column securitystamp  */
    ///Get the value of the column securitystamp, returns the default value if the column is null
    const std::string &getValueOfSecuritystamp() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSecuritystamp() const noexcept;
    ///Set the value of the column securitystamp
    void setSecuritystamp(const std::string &pSecuritystamp) noexcept;
    void setSecuritystamp(std::string &&pSecuritystamp) noexcept;
    void setSecuritystampToNull() noexcept;

    /**  For column displayname  */
    ///Get the value of the column displayname, returns the default value if the column is null
    const std::string &getValueOfDisplayname() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDisplayname() const noexcept;
    ///Set the value of the column displayname
    void setDisplayname(const std::string &pDisplayname) noexcept;
    void setDisplayname(std::string &&pDisplayname) noexcept;
    void setDisplaynameToNull() noexcept;

    /**  For column avatar  */
    ///Get the value of the column avatar, returns the default value if the column is null
    const std::string &getValueOfAvatar() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getAvatar() const noexcept;
    ///Set the value of the column avatar
    void setAvatar(const std::string &pAvatar) noexcept;
    void setAvatar(std::string &&pAvatar) noexcept;
    void setAvatarToNull() noexcept;

    /**  For column type  */
    ///Get the value of the column type, returns the default value if the column is null
    const int32_t &getValueOfType() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getType() const noexcept;
    ///Set the value of the column type
    void setType(const int32_t &pType) noexcept;

    /**  For column accessfailedcount  */
    ///Get the value of the column accessfailedcount, returns the default value if the column is null
    const short &getValueOfAccessfailedcount() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<short> &getAccessfailedcount() const noexcept;
    ///Set the value of the column accessfailedcount
    void setAccessfailedcount(const short &pAccessfailedcount) noexcept;
    void setAccessfailedcountToNull() noexcept;

    /**  For column email  */
    ///Get the value of the column email, returns the default value if the column is null
    const std::string &getValueOfEmail() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getEmail() const noexcept;
    ///Set the value of the column email
    void setEmail(const std::string &pEmail) noexcept;
    void setEmail(std::string &&pEmail) noexcept;
    void setEmailToNull() noexcept;

    /**  For column isemailconfirmed  */
    ///Get the value of the column isemailconfirmed, returns the default value if the column is null
    const std::string &getValueOfIsemailconfirmed() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getIsemailconfirmed() const noexcept;
    ///Set the value of the column isemailconfirmed
    void setIsemailconfirmed(const std::string &pIsemailconfirmed) noexcept;
    void setIsemailconfirmed(std::string &&pIsemailconfirmed) noexcept;
    void setIsemailconfirmedToNull() noexcept;

    /**  For column phonenumber  */
    ///Get the value of the column phonenumber, returns the default value if the column is null
    const std::string &getValueOfPhonenumber() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPhonenumber() const noexcept;
    ///Set the value of the column phonenumber
    void setPhonenumber(const std::string &pPhonenumber) noexcept;
    void setPhonenumber(std::string &&pPhonenumber) noexcept;
    void setPhonenumberToNull() noexcept;

    /**  For column isphonenumberconfirmed  */
    ///Get the value of the column isphonenumberconfirmed, returns the default value if the column is null
    const std::string &getValueOfIsphonenumberconfirmed() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getIsphonenumberconfirmed() const noexcept;
    ///Set the value of the column isphonenumberconfirmed
    void setIsphonenumberconfirmed(const std::string &pIsphonenumberconfirmed) noexcept;
    void setIsphonenumberconfirmed(std::string &&pIsphonenumberconfirmed) noexcept;
    void setIsphonenumberconfirmedToNull() noexcept;

    /**  For column twofactorenabled  */
    ///Get the value of the column twofactorenabled, returns the default value if the column is null
    const std::string &getValueOfTwofactorenabled() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getTwofactorenabled() const noexcept;
    ///Set the value of the column twofactorenabled
    void setTwofactorenabled(const std::string &pTwofactorenabled) noexcept;
    void setTwofactorenabled(std::string &&pTwofactorenabled) noexcept;
    void setTwofactorenabledToNull() noexcept;

    /**  For column sessionid  */
    ///Get the value of the column sessionid, returns the default value if the column is null
    const std::string &getValueOfSessionid() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSessionid() const noexcept;
    ///Set the value of the column sessionid
    void setSessionid(const std::string &pSessionid) noexcept;
    void setSessionid(std::string &&pSessionid) noexcept;
    void setSessionidToNull() noexcept;

    /**  For column status  */
    ///Get the value of the column status, returns the default value if the column is null
    const int32_t &getValueOfStatus() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getStatus() const noexcept;
    ///Set the value of the column status
    void setStatus(const int32_t &pStatus) noexcept;
    void setStatusToNull() noexcept;

    /**  For column privatekey  */
    ///Get the value of the column privatekey, returns the default value if the column is null
    const std::string &getValueOfPrivatekey() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPrivatekey() const noexcept;
    ///Set the value of the column privatekey
    void setPrivatekey(const std::string &pPrivatekey) noexcept;
    void setPrivatekey(std::string &&pPrivatekey) noexcept;
    void setPrivatekeyToNull() noexcept;

    /**  For column isfirsttimelogin  */
    ///Get the value of the column isfirsttimelogin, returns the default value if the column is null
    const std::string &getValueOfIsfirsttimelogin() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getIsfirsttimelogin() const noexcept;
    ///Set the value of the column isfirsttimelogin
    void setIsfirsttimelogin(const std::string &pIsfirsttimelogin) noexcept;
    void setIsfirsttimelogin(std::string &&pIsfirsttimelogin) noexcept;
    void setIsfirsttimeloginToNull() noexcept;

    /**  For column ishrreference  */
    ///Get the value of the column ishrreference, returns the default value if the column is null
    const std::string &getValueOfIshrreference() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getIshrreference() const noexcept;
    ///Set the value of the column ishrreference
    void setIshrreference(const std::string &pIshrreference) noexcept;
    void setIshrreference(std::string &&pIshrreference) noexcept;
    void setIshrreferenceToNull() noexcept;

    /**  For column session  */
    ///Get the value of the column session, returns the default value if the column is null
    const std::string &getValueOfSession() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSession() const noexcept;
    ///Set the value of the column session
    void setSession(const std::string &pSession) noexcept;
    void setSession(std::string &&pSession) noexcept;
    void setSessionToNull() noexcept;

    /**  For column createddate  */
    ///Get the value of the column createddate, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreateddate() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreateddate() const noexcept;
    ///Set the value of the column createddate
    void setCreateddate(const ::trantor::Date &pCreateddate) noexcept;

    /**  For column createdby  */
    ///Get the value of the column createdby, returns the default value if the column is null
    const std::string &getValueOfCreatedby() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getCreatedby() const noexcept;
    ///Set the value of the column createdby
    void setCreatedby(const std::string &pCreatedby) noexcept;
    void setCreatedby(std::string &&pCreatedby) noexcept;
    void setCreatedbyToNull() noexcept;

    /**  For column updateddate  */
    ///Get the value of the column updateddate, returns the default value if the column is null
    const ::trantor::Date &getValueOfUpdateddate() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getUpdateddate() const noexcept;
    ///Set the value of the column updateddate
    void setUpdateddate(const ::trantor::Date &pUpdateddate) noexcept;
    void setUpdateddateToNull() noexcept;

    /**  For column updatedby  */
    ///Get the value of the column updatedby, returns the default value if the column is null
    const std::string &getValueOfUpdatedby() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getUpdatedby() const noexcept;
    ///Set the value of the column updatedby
    void setUpdatedby(const std::string &pUpdatedby) noexcept;
    void setUpdatedby(std::string &&pUpdatedby) noexcept;
    void setUpdatedbyToNull() noexcept;


    static size_t getColumnNumber() noexcept {  return 23;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Systemaccounts>;
    friend drogon::orm::BaseBuilder<Systemaccounts, true, true>;
    friend drogon::orm::BaseBuilder<Systemaccounts, true, false>;
    friend drogon::orm::BaseBuilder<Systemaccounts, false, true>;
    friend drogon::orm::BaseBuilder<Systemaccounts, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Systemaccounts>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> accountid_;
    std::shared_ptr<std::string> username_;
    std::shared_ptr<std::string> passwordhash_;
    std::shared_ptr<std::string> securitystamp_;
    std::shared_ptr<std::string> displayname_;
    std::shared_ptr<std::string> avatar_;
    std::shared_ptr<int32_t> type_;
    std::shared_ptr<short> accessfailedcount_;
    std::shared_ptr<std::string> email_;
    std::shared_ptr<std::string> isemailconfirmed_;
    std::shared_ptr<std::string> phonenumber_;
    std::shared_ptr<std::string> isphonenumberconfirmed_;
    std::shared_ptr<std::string> twofactorenabled_;
    std::shared_ptr<std::string> sessionid_;
    std::shared_ptr<int32_t> status_;
    std::shared_ptr<std::string> privatekey_;
    std::shared_ptr<std::string> isfirsttimelogin_;
    std::shared_ptr<std::string> ishrreference_;
    std::shared_ptr<std::string> session_;
    std::shared_ptr<::trantor::Date> createddate_;
    std::shared_ptr<std::string> createdby_;
    std::shared_ptr<::trantor::Date> updateddate_;
    std::shared_ptr<std::string> updatedby_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[23]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if(dirtyFlag_[0])
        {
            sql += "accountid,";
            ++parametersCount;
        }
        if(dirtyFlag_[1])
        {
            sql += "username,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "passwordhash,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "securitystamp,";
            ++parametersCount;
        }
        if(dirtyFlag_[4])
        {
            sql += "displayname,";
            ++parametersCount;
        }
        if(dirtyFlag_[5])
        {
            sql += "avatar,";
            ++parametersCount;
        }
        if(dirtyFlag_[6])
        {
            sql += "type,";
            ++parametersCount;
        }
        if(dirtyFlag_[7])
        {
            sql += "accessfailedcount,";
            ++parametersCount;
        }
        if(dirtyFlag_[8])
        {
            sql += "email,";
            ++parametersCount;
        }
        if(dirtyFlag_[9])
        {
            sql += "isemailconfirmed,";
            ++parametersCount;
        }
        if(dirtyFlag_[10])
        {
            sql += "phonenumber,";
            ++parametersCount;
        }
        if(dirtyFlag_[11])
        {
            sql += "isphonenumberconfirmed,";
            ++parametersCount;
        }
        if(dirtyFlag_[12])
        {
            sql += "twofactorenabled,";
            ++parametersCount;
        }
        if(dirtyFlag_[13])
        {
            sql += "sessionid,";
            ++parametersCount;
        }
        if(dirtyFlag_[14])
        {
            sql += "status,";
            ++parametersCount;
        }
        if(dirtyFlag_[15])
        {
            sql += "privatekey,";
            ++parametersCount;
        }
        if(dirtyFlag_[16])
        {
            sql += "isfirsttimelogin,";
            ++parametersCount;
        }
        if(dirtyFlag_[17])
        {
            sql += "ishrreference,";
            ++parametersCount;
        }
        if(dirtyFlag_[18])
        {
            sql += "session,";
            ++parametersCount;
        }
        if(dirtyFlag_[19])
        {
            sql += "createddate,";
            ++parametersCount;
        }
        if(dirtyFlag_[20])
        {
            sql += "createdby,";
            ++parametersCount;
        }
        if(dirtyFlag_[21])
        {
            sql += "updateddate,";
            ++parametersCount;
        }
        if(dirtyFlag_[22])
        {
            sql += "updatedby,";
            ++parametersCount;
        }
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        int placeholder=1;
        char placeholderStr[64];
        size_t n=0;
        if(dirtyFlag_[0])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[1])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[2])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[3])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[4])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[5])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[6])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[7])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[8])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[9])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[10])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[11])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[12])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[13])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[14])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[15])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[16])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[17])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[18])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[19])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[20])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[21])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[22])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        if(needSelection)
        {
            sql.append(") returning *");
        }
        else
        {
            sql.append(1, ')');
        }
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace test
} // namespace drogon_model
