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
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hex.h>
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;
#include <cryptopp/scrypt.h>

#include <drogon/drogon.h>

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
        std::string cipher, encoded;

        /*********************************\
        \*********************************/

        try
        {
            // string encoded = "FFEEDDCCBBAA99887766554433221100";
            std::string token = tokenHash;
            if (token.compare("") == 0)
                token = drogon::app().getCustomConfig()["ACCESS_TOKEN_SECRET"].asString();
            // std::string token = "632c4dfa61af1dd6ca5b80f3152d6a6f3c4a8b0fda39fa2a4945e7a7c775110e";
            // 632c4dfa61af1dd6ca5b80f3152d6a6f3c4a8b0fda39fa2a4945e7a7c775110e
            std::string decoded;

            // StringSource ff(encoded, true,
            //     new HexDecoder(
            //         new StringSink(decoded)
            //     ) // HexDecoder
            // ); // StringSource

            // byte byteString[decoded.length()];
            // memcpy(byteString, encoded.data(), decoded.length());

            byte byteString[token.length()];
            memcpy(byteString, token.data(), token.length());

            std::cout << (int)byteString[15] << std::endl;
            CryptoPP::SecByteBlock derived(32);
            std::string salt = "GfG";
            byte saltBytes[salt.length()];
            memcpy(saltBytes, salt.data(), salt.length());

            CryptoPP::Scrypt scrypt;
            scrypt.DeriveKey(derived, derived.size(), (CryptoPP::byte *)byteString, token.length(), (CryptoPP::byte *)saltBytes, salt.length(), 16384, 8, 1);

            byte temp[32];
            memcpy(temp, derived, 32);
            std::string s((const char *)temp, 32);

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
            std::cout << "cipher " << encoded << std::endl;
        }
        catch (const CryptoPP::Exception &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
        return encoded;
    }
    bool matches(const std::string &rawPassword, const std::string &hashPassword, const std::string &hashToken = "")
    {
        return boost::algorithm::to_lower_copy(encrypt(rawPassword, hashToken)).compare(boost::algorithm::to_lower_copy(hashPassword)) == 0;
    }
}