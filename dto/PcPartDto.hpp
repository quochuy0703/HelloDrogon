#pragma once

#include <iostream>
#include <optional>
#include <drogon/drogon.h>
#include "../models/PcPart.h"

using Model = drogon_model::test::PcPart;

namespace app_dto
{
    struct PcPartDto
    {
        int32_t ID;
        std::string COSTCODE;
        std::string PARTCODE;
        std::string PARTNAME;
        int32_t STATUS;
        std::string CREATEUSER;
        std::string CREATEDATE;
        std::string UPDATEUSER;
        std::string UPDATEDATE;

        static Model toModel(PcPartDto data)
        {
            Model model;

            model.setId(data.ID);
            model.setCostcode(data.COSTCODE);
            model.setPartcode(data.PARTCODE);
            model.setPartname(data.PARTNAME);
            model.setStatus(data.STATUS);

            return model;
        }
        static PcPartDto fromModel(Model model)
        {
            PcPartDto data;

            data.ID = model.getValueOfId();
            data.COSTCODE = model.getValueOfCostcode();
            data.PARTCODE = model.getValueOfPartcode();
            data.PARTNAME = model.getValueOfPartname();
            data.STATUS = model.getValueOfStatus();

            return data;
        }

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

namespace drogon
{
    template <>
    inline app_dto::PcPartDto fromRequest(const HttpRequest &req)
    {
        app_dto::PcPartDto data;
        auto json = req.getJsonObject();

        if (json)
        {

            data.ID = (*json)["Id"].asInt();
            data.COSTCODE = (*json)["CostCode"].asString();
            data.PARTCODE = (*json)["PartCode"].asString();
            data.PARTNAME = (*json)["PartName"].asString();
            data.STATUS = (*json)["Status"].asInt();
        }

        return data;
    }
}