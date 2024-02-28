/**
 *
 *  Review.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Review.h"
#include "Course.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::test;

const std::string Review::Cols::_id = "id";
const std::string Review::Cols::_course_id = "course_id";
const std::string Review::Cols::_content = "content";
const std::string Review::primaryKeyName = "id";
const bool Review::hasPrimaryKey = true;
const std::string Review::tableName = "review";

const std::vector<typename Review::MetaData> Review::metaData_={
{"id","int32_t","integer",4,1,1,1},
{"course_id","int32_t","integer",4,0,0,0},
{"content","std::string","character varying",0,0,0,0}
};
const std::string &Review::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Review::Review(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>());
        }
        if(!r["course_id"].isNull())
        {
            courseId_=std::make_shared<int32_t>(r["course_id"].as<int32_t>());
        }
        if(!r["content"].isNull())
        {
            content_=std::make_shared<std::string>(r["content"].as<std::string>());
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
            courseId_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            content_=std::make_shared<std::string>(r[index].as<std::string>());
        }
    }

}

Review::Review(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
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
            courseId_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            content_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}

Review::Review(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("course_id"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["course_id"].isNull())
        {
            courseId_=std::make_shared<int32_t>((int32_t)pJson["course_id"].asInt64());
        }
    }
    if(pJson.isMember("content"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["content"].isNull())
        {
            content_=std::make_shared<std::string>(pJson["content"].asString());
        }
    }
}

void Review::updateByMasqueradedJson(const Json::Value &pJson,
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
            courseId_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            content_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
}

void Review::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<int32_t>((int32_t)pJson["id"].asInt64());
        }
    }
    if(pJson.isMember("course_id"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["course_id"].isNull())
        {
            courseId_=std::make_shared<int32_t>((int32_t)pJson["course_id"].asInt64());
        }
    }
    if(pJson.isMember("content"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["content"].isNull())
        {
            content_=std::make_shared<std::string>(pJson["content"].asString());
        }
    }
}

const int32_t &Review::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Review::getId() const noexcept
{
    return id_;
}
void Review::setId(const int32_t &pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}
const typename Review::PrimaryKeyType & Review::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const int32_t &Review::getValueOfCourseId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(courseId_)
        return *courseId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Review::getCourseId() const noexcept
{
    return courseId_;
}
void Review::setCourseId(const int32_t &pCourseId) noexcept
{
    courseId_ = std::make_shared<int32_t>(pCourseId);
    dirtyFlag_[1] = true;
}
void Review::setCourseIdToNull() noexcept
{
    courseId_.reset();
    dirtyFlag_[1] = true;
}

const std::string &Review::getValueOfContent() const noexcept
{
    const static std::string defaultValue = std::string();
    if(content_)
        return *content_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Review::getContent() const noexcept
{
    return content_;
}
void Review::setContent(const std::string &pContent) noexcept
{
    content_ = std::make_shared<std::string>(pContent);
    dirtyFlag_[2] = true;
}
void Review::setContent(std::string &&pContent) noexcept
{
    content_ = std::make_shared<std::string>(std::move(pContent));
    dirtyFlag_[2] = true;
}
void Review::setContentToNull() noexcept
{
    content_.reset();
    dirtyFlag_[2] = true;
}

void Review::updateId(const uint64_t id)
{
}

const std::vector<std::string> &Review::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "course_id",
        "content"
    };
    return inCols;
}

void Review::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getCourseId())
        {
            binder << getValueOfCourseId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getContent())
        {
            binder << getValueOfContent();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Review::updateColumns() const
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

void Review::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getCourseId())
        {
            binder << getValueOfCourseId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getContent())
        {
            binder << getValueOfContent();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value Review::toJson() const
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
    if(getCourseId())
    {
        ret["course_id"]=getValueOfCourseId();
    }
    else
    {
        ret["course_id"]=Json::Value();
    }
    if(getContent())
    {
        ret["content"]=getValueOfContent();
    }
    else
    {
        ret["content"]=Json::Value();
    }
    return ret;
}

Json::Value Review::toMasqueradedJson(
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
            if(getCourseId())
            {
                ret[pMasqueradingVector[1]]=getValueOfCourseId();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getContent())
            {
                ret[pMasqueradingVector[2]]=getValueOfContent();
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
    if(getCourseId())
    {
        ret["course_id"]=getValueOfCourseId();
    }
    else
    {
        ret["course_id"]=Json::Value();
    }
    if(getContent())
    {
        ret["content"]=getValueOfContent();
    }
    else
    {
        ret["content"]=Json::Value();
    }
    return ret;
}

bool Review::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("course_id"))
    {
        if(!validJsonOfField(1, "course_id", pJson["course_id"], err, true))
            return false;
    }
    if(pJson.isMember("content"))
    {
        if(!validJsonOfField(2, "content", pJson["content"], err, true))
            return false;
    }
    return true;
}
bool Review::validateMasqueradedJsonForCreation(const Json::Value &pJson,
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
bool Review::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
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
    if(pJson.isMember("course_id"))
    {
        if(!validJsonOfField(1, "course_id", pJson["course_id"], err, false))
            return false;
    }
    if(pJson.isMember("content"))
    {
        if(!validJsonOfField(2, "content", pJson["content"], err, false))
            return false;
    }
    return true;
}
bool Review::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
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
bool Review::validJsonOfField(size_t index,
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
            if(!pJson.isInt())
            {
                err="Type error in the "+fieldName+" field";
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
            break;
        default:
            err="Internal error in the server";
            return false;
    }
    return true;
}

Course Review::getCourse(const drogon::orm::DbClientPtr &clientPtr) const {
    std::shared_ptr<std::promise<Course>> pro(new std::promise<Course>);
    std::future<Course> f = pro->get_future();
    getCourse(clientPtr, [&pro] (Course result) {
        try {
            pro->set_value(result);
        }
        catch (...) {
            pro->set_exception(std::current_exception());
        }
    }, [&pro] (const DrogonDbException &err) {
        pro->set_exception(std::make_exception_ptr(err));
    });
    return f.get();
}
void Review::getCourse(const DbClientPtr &clientPtr,
                       const std::function<void(Course)> &rcb,
                       const ExceptionCallback &ecb) const
{
    const static std::string sql = "select * from course where id = $1";
    *clientPtr << sql
               << *courseId_
               >> [rcb = std::move(rcb), ecb](const Result &r){
                    if (r.size() == 0)
                    {
                        ecb(UnexpectedRows("0 rows found"));
                    }
                    else if (r.size() > 1)
                    {
                        ecb(UnexpectedRows("Found more than one row"));
                    }
                    else
                    {
                        rcb(Course(r[0]));
                    }
               }
               >> ecb;
}
