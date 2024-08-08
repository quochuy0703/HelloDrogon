#include "CryptoHelper.hpp"

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <string>

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/modes.h"
using CryptoPP::CBC_Mode;
using CryptoPP::CTR_Mode;

#include "cryptopp/filters.h"
using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include <cryptopp/hmac.h>
using CryptoPP::HMAC;
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hex.h>
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;
#include <cryptopp/scrypt.h>

#include <drogon/drogon.h>

using namespace std;

namespace app_helpers::crypto_helper
{

    std::string encrypt(const std::string &plain, const std::string &tokenHash = "")
    {

        // AutoSeededRandomPool prng;
        // CryptoPP::SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        // prng.GenerateBlock( key, key.size() );

        CryptoPP::byte iv[AES::BLOCKSIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        // prng.GenerateBlock( iv, sizeof(iv) );

        // string plain = "0000000"; //c5ab3c6700c8939f670d3a954f81b609
        string cipher, encoded;

        /*********************************\
        \*********************************/

        try
        {
            // string encoded = "FFEEDDCCBBAA99887766554433221100";
            string token = tokenHash;
            if (token.compare("") == 0)
                token = drogon::app().getCustomConfig()["ACCESS_TOKEN_SECRET"].asString();
            // std::string token = "632c4dfa61af1dd6ca5b80f3152d6a6f3c4a8b0fda39fa2a4945e7a7c775110e";
            // 632c4dfa61af1dd6ca5b80f3152d6a6f3c4a8b0fda39fa2a4945e7a7c775110e
            string decoded;

            // StringSource ff(encoded, true,
            //     new HexDecoder(
            //         new StringSink(decoded)
            //     ) // HexDecoder
            // ); // StringSource

            // byte byteString[decoded.length()];
            // memcpy(byteString, encoded.data(), decoded.length());

            std::vector<CryptoPP::byte> byteString(token.begin(), token.end());

            cout << (int)byteString[15] << endl;
            CryptoPP::SecByteBlock derived(32);
            string salt = "GfG";

            std::vector<CryptoPP::byte> saltBytes(salt.begin(), salt.end());

            CryptoPP::Scrypt scrypt;
            scrypt.DeriveKey(derived, derived.size(), (CryptoPP::byte *)byteString.data(), token.length(), (CryptoPP::byte*)saltBytes.data(), salt.length(), 16384, 8, 1);

            CryptoPP::byte temp[32];
            memcpy(temp, derived, 32);
            string s((const char *)temp, 32);

            encoded.clear();
            StringSource tt(s, true, new HexEncoder(new StringSink(encoded))); // StringSource

            CBC_Mode<AES>::Encryption e;
            e.SetKeyWithIV(derived, derived.size(), iv);

            // The StreamTransformationFilter adds padding
            //  as required. ECB and CBC Mode must be padded
            //  to the block size of the cipher.
            StringSource ss(plain, true,
                            new StreamTransformationFilter(e,
                                                           new StringSink(cipher)) // StreamTransformationFilter
            );                                                                     // StringSource
            // Pretty print
            encoded.clear();
            // Pretty print cipher text
            StringSource cc(cipher, true, new HexEncoder(new StringSink(encoded))); // StringSource
            cout << "cipher " << encoded << endl;
        }
        catch (const CryptoPP::Exception &e)
        {
            cerr << e.what() << endl;
            exit(1);
        }
        return encoded;
    }
    std::string decrypt(const std::string &hashData, const std::string &tokenHash = "")
    {

        CryptoPP::byte iv[AES::BLOCKSIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        std::string cipher, encoded, decoded;

        /*********************************\
        \*********************************/

        try
        {
            std::string token = tokenHash;
            if (token.compare("") == 0)
                token = drogon::app().getCustomConfig()["ACCESS_TOKEN_SECRET"].asString();

            std::vector<CryptoPP::byte> byteString(token.begin(), token.end());

            std::cout << (int)byteString[15] << std::endl;
            CryptoPP::SecByteBlock derived(32);
            std::string salt = "GfG";

            std::vector<CryptoPP::byte> saltBytes(salt.begin(), salt.end());

            CryptoPP::Scrypt scrypt;
            scrypt.DeriveKey(derived, derived.size(), (CryptoPP::byte *)byteString.data(), token.length(), (CryptoPP::byte*)saltBytes.data(), salt.length(), 16384, 8, 1);

            CryptoPP::byte temp[32];
            memcpy(temp, derived, 32);
            std::string s((const char *)temp, 32);

            encoded.clear();
            StringSource tt(s, true, new HexEncoder(new StringSink(encoded))); // StringSource

            // Decrypt
            CBC_Mode<AES>::Decryption e;
            e.SetKeyWithIV(derived, derived.size(), iv);

            // convert to byte
            encoded.clear();
            StringSource ssk(hashData, true /*pumpAll*/,
                             new HexDecoder(
                                 new StringSink(encoded)) // HexDecoder
            );                                            // StringSource

            // The StreamTransformationFilter adds padding
            //  as required. ECB and CBC Mode must be padded
            //  to the block size of the cipher.
            StringSource ss(encoded, true,
                            new StreamTransformationFilter(e,
                                                           new StringSink(decoded)) // StreamTransformationFilter
            );                                                                      // StringSource
            std::cout << "cipher " << decoded << std::endl;
        }
        catch (const CryptoPP::Exception &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
        return decoded;
    }
    bool matches(const std::string &rawPassword, const std::string &hashPassword, const std::string &hashToken = "")
    {
        return boost::algorithm::to_lower_copy(encrypt(rawPassword, hashToken)).compare(boost::algorithm::to_lower_copy(hashPassword)) == 0;
    }
    std::string generateHMAC(const std::string &plain, const std::string &keyPlain)
    {
        CryptoPP::SecByteBlock key;
        if (keyPlain.compare("") == 0)
        {
            CryptoPP::byte stringKey[16];
            key.Assign(stringKey, 16);
            AutoSeededRandomPool prng;
            prng.GenerateBlock(key, key.size());
        }
        else
        {
            std::vector<CryptoPP::byte> stringKey(keyPlain.begin(), keyPlain.end());
            key.Assign(stringKey.data(), stringKey.size());
        }

        // AutoSeededRandomPool prng;

        // CryptoPP::SecByteBlock key(16);
        // prng.GenerateBlock(key, key.size());

        std::string mac, encoded;

        /*********************************\
        \*********************************/

        // Pretty print key
        encoded.clear();
        StringSource(key, key.size(), true,
                     new HexEncoder(
                         new StringSink(encoded)) // HexEncoder
        );                                        // StringSource
        std::cout << "key: " << encoded << std::endl;

        std::cout << "plain text: " << plain << std::endl;

        /*********************************\
        \*********************************/

        try
        {
            HMAC<CryptoPP::SHA256> hmac(key, key.size());

            StringSource(plain, true,
                         new CryptoPP::HashFilter(hmac,
                                                  new StringSink(mac)) // HashFilter
            );                                                         // StringSource
        }
        catch (const CryptoPP::Exception &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }

        /*********************************\
        \*********************************/

        // Pretty print MAC
        encoded.clear();
        StringSource(mac, true,
                     new HexEncoder(
                         new StringSink(encoded)) // HexEncoder
        );                                        // StringSource
        std::cout << "hmac: " << encoded << std::endl;

        // /*********************************\
        // \*********************************/

        // try
        // {
        //     CryptoPP::HMAC<CryptoPP::SHA256> hmac(key, key.size());
        //     const int flags = CryptoPP::HashVerificationFilter::THROW_EXCEPTION | CryptoPP::HashVerificationFilter::HASH_AT_END;

        //     // Tamper with message
        //     // plain[0] ^= 0x01;

        //     // Tamper with MAC
        //     // mac[0] ^= 0x01;

        //     StringSource(plain + mac, true,
        //                  new CryptoPP::HashVerificationFilter(hmac, NULL, flags)); // StringSource

        //     std::cout << "Verified message" << std::endl;
        // }
        // catch (const CryptoPP::Exception &e)
        // {
        //     std::cerr << e.what() << std::endl;
        //     exit(1);
        // }
        return encoded;
    }

    app_helpers::execute_awaiter::ExecuteAwaiterReturn<std::string> generateHMACCoro(const std::string &plain, const std::string &keyPlain)
    {
        return app_helpers::execute_awaiter::executeIntensiveFunctionReturn<std::string>([&]()
                                                                                         { return app_helpers::crypto_helper::generateHMAC(plain, keyPlain); });
    }
}