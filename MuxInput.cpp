/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                    */
/*    FILE: MuxInput.cpp                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include "MuxInput.hpp"
#include "Mux.h"
#include "rapidjson/rapidjson.h"
#include <vector>
#include <memory>
#include <cstdlib>

using namespace std;
using namespace rapidjson;

std::unique_ptr<MuxInputBase> MuxInputBase::muxInputFactory (rapidjson::Value &d) {
    string type = d["inputType"].GetString();
    if ("STRING" == type) {
        return MuxInputString::muxInputStringFactory(d);
    } else if ("DOUBLE" == type) {
        return MuxInputDouble::muxInputDoubleFactory(d);
    } else if ("BINARY" == type) {
        return MuxInputBinary::muxInputBinaryFactory(d);
    }
    return unique_ptr<MuxInputBase>(nullptr);
}

std::unique_ptr<MuxInputString> MuxInputString::muxInputStringFactory (rapidjson::Value &d) {
    if (d["inputType"].GetString() != "STRING") return unique_ptr<MuxInputString>(nullptr);
    string func = d["function"].GetString();
    if ("fixed" == func) {
        return unique_ptr<MuxInputString>(new MuxInputStringConst(d));
    } else if ("variable" == func) {
        return unique_ptr<MuxInputString>(new MuxInputStringVar(d));
    }
    return unique_ptr<MuxInputString>(nullptr);
}

bool MuxInputString::isTrue() {
    return (("T" == val) || ("t" == val) || ("true" == val) || ("TRUE" == val));
}

MuxInputStringVar::MuxInputStringVar(rapidjson::Value &d) {
    var = d["var"].GetString();
    if (d.HasMember("val")) val = d["val"].GetString();
}

bool MuxInputStringVar::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == var) && (msg.IsString())) {
        val = msg.GetString();
        return true;
    } else {
        return false;
    }
}

bool MuxInputStringVar::subscribe(Mux *m) {return m->registerVar(var);}

MuxInputStringConst::MuxInputStringConst(rapidjson::Value &d) {
    var = "";
    if (d.HasMember("val")) {
        val = d["val"].GetString();
    } else val = "";
}

std::unique_ptr<MuxInputDouble> MuxInputDouble::muxInputDoubleFactory (rapidjson::Value &d) {
    if (d["inputType"].GetString() != "DOUBLE") return unique_ptr<MuxInputDouble>(nullptr);
    string func = d["function"].GetString();
    if ("fixed" == func) {
        return unique_ptr<MuxInputDouble>(new MuxInputDoubleConst(d));
    } else if ("variable" == func) {
        return unique_ptr<MuxInputDouble>(new MuxInputDoubleVar(d));
    }
    return unique_ptr<MuxInputDouble>(nullptr);
}

MuxInputDoubleVar::MuxInputDoubleVar(rapidjson::Value &d) {
    var = d["var"].GetString();
    if (d.HasMember("val")) val = d["val"].GetFloat();
    if (d.HasMember("threshold")) val = d["threshold"].GetFloat();
}

bool MuxInputDoubleVar::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == var) && (msg.IsDouble())) {
        val = msg.GetDouble();
        return true;
    } else {
        return false;
    }
}

bool MuxInputDoubleVar::subscribe(Mux *m) {return m->registerVar(var);}

MuxInputDoubleConst::MuxInputDoubleConst(rapidjson::Value &d) {
    var = "";
    if (d.HasMember("val")) val = d["val"].GetFloat();
    if (d.HasMember("threshold")) val = d["threshold"].GetFloat();
}

std::unique_ptr<MuxInputBinary> MuxInputBinary::muxInputBinaryFactory (rapidjson::Value &d) {
    if (d["inputType"].GetString() != "BINARY") return unique_ptr<MuxInputBinary>(nullptr);
    string func = d["function"].GetString();
    if ("fixed" == func) {
        return unique_ptr<MuxInputBinary>(new MuxInputBinaryConst(d));
    } else if ("variable" == func) {
        return unique_ptr<MuxInputBinary>(new MuxInputBinaryVar(d));
    }
    return unique_ptr<MuxInputBinary>(nullptr);
}

bool MuxInputBinary::loadString(rapidjson::Value &d) {
    if (d.IsString()) {
        string tmp = d.GetString();
        val.clear();
        val.resize(tmp.size());
        for (auto &c: tmp) val.push_back(static_cast<uint8_t>(c));
        return true;
    } else return false;
}

bool MuxInputBinary::loadArray(rapidjson::Value &d) {
    if (d.IsArray()) {
        val.clear();
        val.resize(d.Size());
        for (auto &c: d.GetArray()) {
            val.push_back(static_cast<uint8_t>(c.GetInt()));
        }
        return true;
    } else return false;
}

bool MuxInputBinary::isTrue() {
    if ((val.size() > 0) && val[0]) return true;
    return false;
}

MuxInputBinaryVar::MuxInputBinaryVar(rapidjson::Value &d) {
    var = d["var"].GetString();
    if (d.HasMember("val")) {
        if (d["val"].IsString()) {
            loadString(d["val"]);
        } else if (d["val"].IsArray()) {
            loadArray(d["val"]);
        }
    }
}

bool MuxInputBinaryVar::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == var) && (msg.IsBinary())) {
        val.clear();
        msg.GetBinaryData(val);
        return true;
    } else {
        return false;
    }
}

bool MuxInputBinaryVar::subscribe(Mux *m) {return m->registerVar(var);}

MuxInputBinaryConst::MuxInputBinaryConst(rapidjson::Value &d) {
    var = "";
    if (d.HasMember("val")) {
        if (d["val"].IsString()) {
            loadString(d["val"]);
        } else if (d["val"].IsArray()) {
            loadArray(d["val"]);
        }
    }
}
