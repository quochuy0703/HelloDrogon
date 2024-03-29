/**
 *
 *  User.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "User.h"
#include "Course.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_test;

const std::string User::Cols::_id = "id";
const std::string User::Cols::_name = "name";
const std::string User::Cols::_email = "email";
const std::string User::Cols::_password = "password";
const std::string User::Cols::_security_stamp = "security_stamp";
const std::string User::primaryKeyName = "id";
const bool User::hasPrimaryKey = true;
const std::string User::tableName = "user";

const std::vector<typename User::MetaData> User::metaData_={
{"id","int32_t","int(11)",4,1,1,1},
{"name","std::string","varchar(100)",100,0,0,0},
{"email","std::string","varchar(45)",45,0,0,0},
{"password","std::string","varchar(64)",64,0,0,0},
{"security_stamp","std::string","longtext",0,0,0,0}
};
const std::string &User::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
User::User(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>());
        }
        if(!r["name"].isNull())
        {
            name_=std::make_shared<std::string>(r["name"].as<std::string>());
        }
        if(!r["email"].isNull())
        {
            email_=std::make_shared<std::string>(r["email"].as<std::string>());
        }
        if(!r["password"].isNull())
        {
            password_=std::make_shared<std::string>(r["password"].as<std::string>());
        }
        if(!r["security_stamp"].isNull())
        {
            securityStamp_=std::make_shared<std::string>(r["security_stamp"].as<std::string>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 5 > r.size())
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
            name_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            email_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 3;
        if(!r[index].isNull())
        {
            password_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 4;
        if(!r[index].isNull())
        {
            securityStamp_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

User::User(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 5)
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
            name_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            password_=std::make_shared<std::string>(pJson[pMasqueradingVector[3]].asString());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            securityStamp_=std::make_shared<std::string>(pJson[pMasqueradingVector[4]].asString());
        }
    }
}

User::User(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("name"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["name"].isNull())
        {
            name_=std::make_shared<std::string>(pJson["name"].asString());
        }
    }
    if(pJson.isMember("email"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("password"))
    {
        dirtyFlag_[3]=true;
        if(!pJson["password"].isNull())
        {
            password_=std::make_shared<std::string>(pJson["password"].asString());
        }
    }
    if(pJson.isMember("security_stamp"))
    {
        dirtyFlag_[4]=true;
        if(!pJson["security_stamp"].isNull())
        {
            securityStamp_=std::make_shared<std::string>(pJson["security_stamp"].asString());
        }
    }
}

void User::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 5)
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
            name_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            email_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            password_=std::make_shared<std::string>(pJson[pMasqueradingVector[3]].asString());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            securityStamp_=std::make_shared<std::string>(pJson[pMasqueradingVector[4]].asString());
        }
    }
}

void User::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("name"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["name"].isNull())
        {
            name_=std::make_shared<std::string>(pJson["name"].asString());
        }
    }
    if(pJson.isMember("email"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["email"].isNull())
        {
            email_=std::make_shared<std::string>(pJson["email"].asString());
        }
    }
    if(pJson.isMember("password"))
    {
        dirtyFlag_[3] = true;
        if(!pJson["password"].isNull())
        {
            password_=std::make_shared<std::string>(pJson["password"].asString());
        }
    }
    if(pJson.isMember("security_stamp"))
    {
        dirtyFlag_[4] = true;
        if(!pJson["security_stamp"].isNull())
        {
            securityStamp_=std::make_shared<std::string>(pJson["security_stamp"].asString());
        }
    }
}

const int32_t &User::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &User::getId() const noexcept
{
    return id_;
}
void User::setId(const int32_t &pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}
const typename User::PrimaryKeyType & User::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string &User::getValueOfName() const noexcept
{
    const static std::string defaultValue = std::string();
    if(name_)
        return *name_;
    return defaultValue;
}
const std::shared_ptr<std::string> &User::getName() const noexcept
{
    return name_;
}
void User::setName(const std::string &pName) noexcept
{
    name_ = std::make_shared<std::string>(pName);
    dirtyFlag_[1] = true;
}
void User::setName(std::string &&pName) noexcept
{
    name_ = std::make_shared<std::string>(std::move(pName));
    dirtyFlag_[1] = true;
}
void User::setNameToNull() noexcept
{
    name_.reset();
    dirtyFlag_[1] = true;
}

const std::string &User::getValueOfEmail() const noexcept
{
    const static std::string defaultValue = std::string();
    if(email_)
        return *email_;
    return defaultValue;
}
const std::shared_ptr<std::string> &User::getEmail() const noexcept
{
    return email_;
}
void User::setEmail(const std::string &pEmail) noexcept
{
    email_ = std::make_shared<std::string>(pEmail);
    dirtyFlag_[2] = true;
}
void User::setEmail(std::string &&pEmail) noexcept
{
    email_ = std::make_shared<std::string>(std::move(pEmail));
    dirtyFlag_[2] = true;
}
void User::setEmailToNull() noexcept
{
    email_.reset();
    dirtyFlag_[2] = true;
}

const std::string &User::getValueOfPassword() const noexcept
{
    const static std::string defaultValue = std::string();
    if(password_)
        return *password_;
    return defaultValue;
}
const std::shared_ptr<std::string> &User::getPassword() const noexcept
{
    return password_;
}
void User::setPassword(const std::string &pPassword) noexcept
{
    password_ = std::make_shared<std::string>(pPassword);
    dirtyFlag_[3] = true;
}
void User::setPassword(std::string &&pPassword) noexcept
{
    password_ = std::make_shared<std::string>(std::move(pPassword));
    dirtyFlag_[3] = true;
}
void User::setPasswordToNull() noexcept
{
    password_.reset();
    dirtyFlag_[3] = true;
}

const std::string &User::getValueOfSecurityStamp() const noexcept
{
    const static std::string defaultValue = std::string();
    if(securityStamp_)
        return *securityStamp_;
    return defaultValue;
}
const std::shared_ptr<std::string> &User::getSecurityStamp() const noexcept
{
    return securityStamp_;
}
void User::setSecurityStamp(const std::string &pSecurityStamp) noexcept
{
    securityStamp_ = std::make_shared<std::string>(pSecurityStamp);
    dirtyFlag_[4] = true;
}
void User::setSecurityStamp(std::string &&pSecurityStamp) noexcept
{
    securityStamp_ = std::make_shared<std::string>(std::move(pSecurityStamp));
    dirtyFlag_[4] = true;
}
void User::setSecurityStampToNull() noexcept
{
    securityStamp_.reset();
    dirtyFlag_[4] = true;
}

void User::updateId(const uint64_t id)
{
    id_ = std::make_shared<int32_t>(static_cast<int32_t>(id));
}

const std::vector<std::string> &User::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "name",
        "email",
        "password",
        "security_stamp"
    };
    return inCols;
}

void User::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getName())
        {
            binder << getValueOfName();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
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
    if(dirtyFlag_[3])
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
    if(dirtyFlag_[4])
    {
        if(getSecurityStamp())
        {
            binder << getValueOfSecurityStamp();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> User::updateColumns() const
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
    if(dirtyFlag_[3])
    {
        ret.push_back(getColumnName(3));
    }
    if(dirtyFlag_[4])
    {
        ret.push_back(getColumnName(4));
    }
    return ret;
}

void User::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getName())
        {
            binder << getValueOfName();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
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
    if(dirtyFlag_[3])
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
    if(dirtyFlag_[4])
    {
        if(getSecurityStamp())
        {
            binder << getValueOfSecurityStamp();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value User::toJson() const
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
    if(getName())
    {
        ret["name"]=getValueOfName();
    }
    else
    {
        ret["name"]=Json::Value();
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
    if(getSecurityStamp())
    {
        ret["security_stamp"]=getValueOfSecurityStamp();
    }
    else
    {
        ret["security_stamp"]=Json::Value();
    }
    return ret;
}

Json::Value User::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 5)
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
            if(getName())
            {
                ret[pMasqueradingVector[1]]=getValueOfName();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getEmail())
            {
                ret[pMasqueradingVector[2]]=getValueOfEmail();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[3].empty())
        {
            if(getPassword())
            {
                ret[pMasqueradingVector[3]]=getValueOfPassword();
            }
            else
            {
                ret[pMasqueradingVector[3]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[4].empty())
        {
            if(getSecurityStamp())
            {
                ret[pMasqueradingVector[4]]=getValueOfSecurityStamp();
            }
            else
            {
                ret[pMasqueradingVector[4]]=Json::Value();
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
    if(getName())
    {
        ret["name"]=getValueOfName();
    }
    else
    {
        ret["name"]=Json::Value();
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
    if(getSecurityStamp())
    {
        ret["security_stamp"]=getValueOfSecurityStamp();
    }
    else
    {
        ret["security_stamp"]=Json::Value();
    }
    return ret;
}

bool User::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("name"))
    {
        if(!validJsonOfField(1, "name", pJson["name"], err, true))
            return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(2, "email", pJson["email"], err, true))
            return false;
    }
    if(pJson.isMember("password"))
    {
        if(!validJsonOfField(3, "password", pJson["password"], err, true))
            return false;
    }
    if(pJson.isMember("security_stamp"))
    {
        if(!validJsonOfField(4, "security_stamp", pJson["security_stamp"], err, true))
            return false;
    }
    return true;
}
bool User::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                              const std::vector<std::string> &pMasqueradingVector,
                                              std::string &err)
{
    if(pMasqueradingVector.size() != 5)
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
      if(!pMasqueradingVector[3].empty())
      {
          if(pJson.isMember(pMasqueradingVector[3]))
          {
              if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, true))
                  return false;
          }
      }
      if(!pMasqueradingVector[4].empty())
      {
          if(pJson.isMember(pMasqueradingVector[4]))
          {
              if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, true))
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
bool User::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
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
    if(pJson.isMember("name"))
    {
        if(!validJsonOfField(1, "name", pJson["name"], err, false))
            return false;
    }
    if(pJson.isMember("email"))
    {
        if(!validJsonOfField(2, "email", pJson["email"], err, false))
            return false;
    }
    if(pJson.isMember("password"))
    {
        if(!validJsonOfField(3, "password", pJson["password"], err, false))
            return false;
    }
    if(pJson.isMember("security_stamp"))
    {
        if(!validJsonOfField(4, "security_stamp", pJson["security_stamp"], err, false))
            return false;
    }
    return true;
}
bool User::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector,
                                            std::string &err)
{
    if(pMasqueradingVector.size() != 5)
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
      if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
      {
          if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, false))
              return false;
      }
      if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
      {
          if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, false))
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
bool User::validJsonOfField(size_t index,
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
            if(pJson.isString() && pJson.asString().length() > 100)
            {
                err="String length exceeds limit for the " +
                    fieldName +
                    " field (the maximum value is 100)";
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
            if(pJson.isString() && pJson.asString().length() > 45)
            {
                err="String length exceeds limit for the " +
                    fieldName +
                    " field (the maximum value is 45)";
                return false;
            }

            break;
        case 3:
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
        case 4:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        default:
            err="Internal error in the server";
            return false;
    }
    return true;
}
std::vector<Course> User::getCourse(const DbClientPtr &clientPtr) const {
    const static std::string sql = "select * from course where instructor_id = ?";
    Result r(nullptr);
    {
        auto binder = *clientPtr << sql;
        binder << *id_ << Mode::Blocking >>
            [&r](const Result &result) { r = result; };
        binder.exec();
    }
    std::vector<Course> ret;
    ret.reserve(r.size());
    for (auto const &row : r)
    {
        ret.emplace_back(Course(row));
    }
    return ret;
}

void User::getCourse(const DbClientPtr &clientPtr,
                     const std::function<void(std::vector<Course>)> &rcb,
                     const ExceptionCallback &ecb) const
{
    const static std::string sql = "select * from course where instructor_id = ?";
    *clientPtr << sql
               << *id_
               >> [rcb = std::move(rcb)](const Result &r){
                   std::vector<Course> ret;
                   ret.reserve(r.size());
                   for (auto const &row : r)
                   {
                       ret.emplace_back(Course(row));
                   }
                   rcb(ret);
               }
               >> ecb;
}
