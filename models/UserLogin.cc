/**
 *
 *  UserLogin.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "UserLogin.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::test;

const std::string UserLogin::Cols::_id = "id";
const std::string UserLogin::Cols::_email = "email";
const std::string UserLogin::Cols::_password = "password";
const std::string UserLogin::primaryKeyName = "id";
const bool UserLogin::hasPrimaryKey = true;
const std::string UserLogin::tableName = "user_login";

const std::vector<typename UserLogin::MetaData> UserLogin::metaData_={
{"id","int32_t","integer",4,1,1,1},
{"email","std::string","character varying",45,0,0,0},
{"password","std::string","character varying",64,0,0,0}
};
const std::string &UserLogin::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
UserLogin::UserLogin(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>());
        }
        if(!r["email"].isNull())
        {
            email_=std::make_shared<std::string>(r["email"].as<std::string>());
        }
        if(!r["password"].isNull())
        {
            password_=std::make_shared<std::string>(r["password"].as<std::string>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 3 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            id_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            email_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            password_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

UserLogin::UserLogin(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            password_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}

UserLogin::UserLogin(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("email"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("password"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["password"].isNull())
        {
            password_=std::make_shared<std::string>(pJson["password"].asString());
        }
    }
}

void UserLogin::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            password_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}

void UserLogin::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("email"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("password"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["password"].isNull())
        {
            password_=std::make_shared<std::string>(pJson["password"].asString());
        }
    }
}

const int32_t &UserLogin::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &UserLogin::getId() const noexcept
{
    return id_;
}
void UserLogin::setId(const int32_t &pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}
const typename UserLogin::PrimaryKeyType & UserLogin::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string &UserLogin::getValueOfEmail() const noexcept
{
    const static std::string defaultValue = std::string();
    if(email_)
        return *email_;
    return defaultValue;
}
const std::shared_ptr<std::string> &UserLogin::getEmail() const noexcept
{
    return email_;
}
void UserLogin::setEmail(const std::string &pEmail) noexcept
{
    email_ = std::make_shared<std::string>(pEmail);
    dirtyFlag_[1] = true;
}
void UserLogin::setEmail(std::string &&pEmail) noexcept
{
    email_ = std::make_shared<std::string>(std::move(pEmail));
    dirtyFlag_[1] = true;
}
void UserLogin::setEmailToNull() noexcept
{
    email_.reset();
    dirtyFlag_[1] = true;
}

const std::string &UserLogin::getValueOfPassword() const noexcept
{
    const static std::string defaultValue = std::string();
    if(password_)
        return *password_;
    return defaultValue;
}
const std::shared_ptr<std::string> &UserLogin::getPassword() const noexcept
{
    return password_;
}
void UserLogin::setPassword(const std::string &pPassword) noexcept
{
    password_ = std::make_shared<std::string>(pPassword);
    dirtyFlag_[2] = true;
}
void UserLogin::setPassword(std::string &&pPassword) noexcept
{
    password_ = std::make_shared<std::string>(std::move(pPassword));
    dirtyFlag_[2] = true;
}
void UserLogin::setPasswordToNull() noexcept
{
    password_.reset();
    dirtyFlag_[2] = true;
}

void UserLogin::updateId(const uint64_t id)
{
}

const std::vector<std::string> &UserLogin::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "email",
        "password"
    };
    return inCols;
}

void UserLogin::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getEmail())
        {
            binder << getValueOfEmail();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getPassword())
        {
            binder << getValueOfPassword();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> UserLogin::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
    {
        ret.push_back(getColumnName(1));
    }
    if(dirtyFlag_[2])
    {
        ret.push_back(getColumnName(2));
    }
    return ret;
}

void UserLogin::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getEmail())
        {
            binder << getValueOfEmail();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getPassword())
        {
            binder << getValueOfPassword();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value UserLogin::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getEmail())
    {
        ret["email"]=getValueOfEmail();
    }
    else
    {
        ret["email"]=Json::Value();
    }
    if(getPassword())
    {
        ret["password"]=getValueOfPassword();
    }
    else
    {
        ret["password"]=Json::Value();
    }
    return ret;
}

Json::Value UserLogin::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 3)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getId())
            {
                ret[pMasqueradingVector[0]]=getValueOfId();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getEmail())
            {
                ret[pMasqueradingVector[1]]=getValueOfEmail();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getPassword())
            {
                ret[pMasqueradingVector[2]]=getValueOfPassword();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId())
    {
        ret["id"]=getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getEmail())
    {
        ret["email"]=getValueOfEmail();
    }
    else
    {
        ret["email"]=Json::Value();
    }
    if(getPassword())
    {
        ret["password"]=getValueOfPassword();
    }
    else
    {
        ret["password"]=Json::Value();
    }
    return ret;
}

bool UserLogin::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(1, "email", pJson["email"], err, true))
            return false;
    }
    if(pJson.isMember("password"))
    {
        if(!validJsonOfField(2, "password", pJson["password"], err, true))
            return false;
    }
    return true;
}
bool UserLogin::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                   const std::vector<std::string> &pMasqueradingVector,
                                                   std::string &err)
{
    if(pMasqueradingVector.size() != 3)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty())
      {
          if(pJson.isMember(pMasqueradingVector[0]))
          {
              if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true))
                  return false;
          }
      }
      if(!pMasqueradingVector[1].empty())
      {
          if(pJson.isMember(pMasqueradingVector[1]))
          {
              if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true))
                  return false;
          }
      }
      if(!pMasqueradingVector[2].empty())
      {
          if(pJson.isMember(pMasqueradingVector[2]))
          {
              if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true))
                  return false;
          }
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool UserLogin::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(1, "email", pJson["email"], err, false))
            return false;
    }
    if(pJson.isMember("password"))
    {
        if(!validJsonOfField(2, "password", pJson["password"], err, false))
            return false;
    }
    return true;
}
bool UserLogin::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                                 const std::vector<std::string> &pMasqueradingVector,
                                                 std::string &err)
{
    if(pMasqueradingVector.size() != 3)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
      {
          if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
              return false;
      }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
      if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
      {
          if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
              return false;
      }
      if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
      {
          if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false))
              return false;
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool UserLogin::validJsonOfField(size_t index,
                                 const std::string &fieldName,
                                 const Json::Value &pJson,
                                 std::string &err,
                                 bool isForCreation)
{
    switch(index)
    {
        case 0:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(isForCreation)
            {
                err="The automatic primary key cannot be set";
                return false;
            }
            if(!pJson.isInt())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 1:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            // asString().length() creates a string object, is there any better way to validate the length?
            if(pJson.isString() && pJson.asString().length() > 45)
            {
                err="String length exceeds limit for the " +
                    fieldName +
                    " field (the maximum value is 45)";
                return false;
            }

            break;
        case 2:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            // asString().length() creates a string object, is there any better way to validate the length?
            if(pJson.isString() && pJson.asString().length() > 64)
            {
                err="String length exceeds limit for the " +
                    fieldName +
                    " field (the maximum value is 64)";
                return false;
            }

            break;
        default:
            err="Internal error in the server";
            return false;
    }
    return true;
}
