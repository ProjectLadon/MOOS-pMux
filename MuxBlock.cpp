/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                    */
/*    FILE: MuxBlock.cpp                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include "MuxBlock.hpp"
#include "Mux.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "schema/block_schema.h"
#include <vector>
#include <memory>
#include <cstdlib>

using namespace std;
using namespace rapidjson;

std::unique_ptr<MuxBlockBase> MuxBlockBase::muxBlockFactory (rapidjson::Value &d) {
    Document sd;
    // check that we can load and parse the
    if (sd.Parse(reinterpret_cast<char*>(block_schema_json), block_schema_json_len).HasParseError()) {
        cerr << "Parse error " << GetParseError_En(sd.GetParseError());
        cerr << " in JSON schema at offset " << sd.GetErrorOffset() << endl;
        std::abort();
    }
    SchemaDocument schemadoc(sd);
    SchemaValidator validator(schemadoc);
    if (d.Accept(validator)) {
        if ("STRING" == d["ChannelType"].GetString()) {
            return unique_ptr<MuxBlockBase>(new MuxBlockString(d));
        } else if ("DOUBLE" == d["ChannelType"].GetString()) {
            return unique_ptr<MuxBlockBase>(new MuxBlockDouble(d));
        } else if ("BINARY" == d["ChannelType"].GetString()) {
            return unique_ptr<MuxBlockBase>(new MuxBlockBinary(d));
        }
    } else {
        StringBuffer buf;
        Writer<StringBuffer> writer(buf);
        d.Accept(writer);
        cerr << "JSON failed validation:" << endl;
        cerr << "\t" << buf.GetString() << endl;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(buf);
        cerr << "Invalid schema: " << buf.GetString() << endl;
        cerr << "Invalid keyword: " << validator.GetInvalidSchemaKeyword() << endl;
        return unique_ptr<MuxBlockBase>(nullptr);
    }
}

bool MuxBlockBase::procMail (CMOOSMsg &msg) {
    bool result = false;
    result |= switchItem->procMail(msg);
    result |= trueVal->procMail(msg);
    result |= falseVal->procMail(msg);
    if (result) fresh = true;
    return result;
}

std::list<std::string> MuxBlockBase::buildReportHeader() {
    list<string> header;
    header.push_back("Out: " + output);
    header.push_back("True: " + trueVal->getVar());
    header.push_back("False: " + falseVal->getVar());
    return header;
}

MuxBlockString::MuxBlockString(rapidjson::Value &d) {
    output = d["Output"].GetString();
    switchItem = MuxInputBase::muxInputFactory(d["Switch"]);
    trueVal = MuxInputString::muxInputStringFactory(d["True"]);
    falseVal = MuxInputString::muxInputStringFactory(d["False"]);
}

bool MuxBlockString::transmit(Mux *m) {
    if (fresh) {
        fresh = false;
        if (switchItem->isTrue()) {
            MuxInputString *v = static_cast<MuxInputString*>(trueVal.get());
            return m->notify(output, v->getVal());
        } else {
            MuxInputString *v = static_cast<MuxInputString*>(falseVal.get());
            return m->notify(output, v->getVal());
        }
    } else return false;
}

std::list<std::string> MuxBlockString::buildReportLines() {
    list<string> value;
    if (switchItem->isTrue()) {
        value.push_back(static_cast<MuxInputString*>(trueVal.get())->getVal());
    } else {
        value.push_back(static_cast<MuxInputString*>(falseVal.get())->getVal());
    }
    value.push_back(static_cast<MuxInputString*>(trueVal.get())->getVal());
    value.push_back(static_cast<MuxInputString*>(falseVal.get())->getVal());
    return value;
}

MuxBlockDouble::MuxBlockDouble(rapidjson::Value &d) {
    output = d["Output"].GetString();
    switchItem = MuxInputBase::muxInputFactory(d["Switch"]);
    trueVal = MuxInputDouble::muxInputDoubleFactory(d["True"]);
    falseVal = MuxInputDouble::muxInputDoubleFactory(d["False"]);
}

bool MuxBlockDouble::transmit(Mux *m) {
    if (fresh) {
        fresh = false;
        if (switchItem->isTrue()) {
            MuxInputDouble *v = static_cast<MuxInputDouble*>(trueVal.get());
            return m->notify(output, v->getVal());
        } else {
            MuxInputDouble *v = static_cast<MuxInputDouble*>(falseVal.get());
            return m->notify(output, v->getVal());
        }
    } else return false;
}

std::list<std::string> MuxBlockDouble::buildReportLines() {
    list<string> value;
    if (switchItem->isTrue()) {
        value.push_back(to_string(static_cast<MuxInputDouble*>(trueVal.get())->getVal()));
    } else {
        value.push_back(to_string(static_cast<MuxInputDouble*>(falseVal.get())->getVal()));
    }
    value.push_back(to_string(static_cast<MuxInputDouble*>(trueVal.get())->getVal()));
    value.push_back(to_string(static_cast<MuxInputDouble*>(falseVal.get())->getVal()));
    return value;
}

MuxBlockBinary::MuxBlockBinary(rapidjson::Value &d) {
    output = d["Output"].GetString();
    switchItem = MuxInputBase::muxInputFactory(d["Switch"]);
    trueVal = MuxInputBinary::muxInputBinaryFactory(d["True"]);
    falseVal = MuxInputBinary::muxInputBinaryFactory(d["False"]);
}

bool MuxBlockBinary::transmit(Mux *m) {
    if (fresh) {
        fresh = false;
        if (switchItem->isTrue()) {
            MuxInputBinary *v = static_cast<MuxInputBinary*>(trueVal.get());
            return m->notify(output, v->getVal());
        } else {
            MuxInputBinary *v = static_cast<MuxInputBinary*>(falseVal.get());
            return m->notify(output, v->getVal());
        }
    } else return false;
}

std::list<std::string> MuxBlockBinary::buildReportLines() {
    list<string> value;
    value.push_back("<binary>");
    value.push_back("<binary>");
    value.push_back("<binary>");
    return value;
}
