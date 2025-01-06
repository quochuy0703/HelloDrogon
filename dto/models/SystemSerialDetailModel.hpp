#pragma once

#include <iostream>
#include <optional>
#include <drogon/drogon.h>
#include "../models/SystemSerialDetail.h"

namespace app_dto::models
{

    struct SystemSerialDetailModel
    {

        std::string code;
        int32_t LineNo;
        trantor::Date start_date;
        std::string start_serial_no;
        std::string prefix;
        std::string suffix;
        int32_t step;
        std::string last_serial_no;
        trantor::Date last_used_date;
        trantor::Date created_date;
        std::string created_by;
        trantor::Date updated_date;
        std::string updated_by;

        // static Model toModel(SystemSerialDetailDto data)
        // {
        //     Model model;

        //     model.setCode(data.code);
        //     model.setLineno(data.LineNo);
        //     model.setStartDate(data.start_date);
        //     model.setStartSerialNo(data.start_serial_no);
        //     model.setPrefix(data.prefix);
        //     model.setSuffix(data.suffix);
        //     model.setStep(data.step);
        //     model.setLastSerialNo(data.last_serial_no);
        //     model.setLastUsedDate(data.last_used_date);
        //     model.setCreatedDate(data.created_date);
        //     model.setCreatedBy(data.created_by);
        //     model.setUpdatedDate(data.updated_date);
        //     model.setUpdatedBy(data.updated_by);

        //     return model;
        // }
        // static SystemSerialDetailDto fromModel(Model model)
        // {
        //     SystemSerialDetailDto data;

        //     data.code = model.getValueOfCode();
        //     data.LineNo = model.getValueOfLineno();
        //     data.start_date = model.getValueOfStartDate();
        //     data.start_serial_no = model.getValueOfStartSerialNo();
        //     data.prefix = model.getValueOfPrefix();
        //     data.suffix = model.getValueOfSuffix();
        //     data.step = model.getValueOfStep();
        //     data.last_serial_no = model.getValueOfLastSerialNo();
        //     data.last_used_date = model.getValueOfLastUsedDate();
        //     data.created_date = model.getValueOfCreatedDate();
        //     data.created_by = model.getValueOfCreatedBy();
        //     data.updated_date = model.getValueOfUpdatedDate();
        //     data.updated_by = model.getValueOfUpdatedBy();

        //     return data;
        // }

        // static PcPartDto toDto(Json::Value root)
        // {
        //     PcPartDto data;
        //     data.userId = root["UserId"].asString();
        //     data.username = root["UserName"].asString();
        //     data.email = root["Email"].asString();

        //     return data;
        // }
    };

}
