/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                */
/*    FILE: Mux.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include <memory>
#include "MBUtils.h"
#include "ACTable.h"
#include "Mux.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

using namespace std;

//---------------------------------------------------------
// Procedure: OnNewMail
bool Mux::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    for(auto &msg: NewMail) {
        string key = msg.GetKey();
        bool result = false;

        for (auto &b: blocks) {
            result |= b->procMail(msg);
        }

        if(!result || (key != "APPCAST_REQ")) { // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
        }
    }

    for (auto &b: blocks) {
        b->transmit(this);
    }

    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer
bool Mux::OnConnectToServer() {
    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
bool Mux::Iterate() {
    AppCastingMOOSApp::Iterate();
    // Do your thing here!
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open
bool Mux::OnStartUp() {
    AppCastingMOOSApp::OnStartUp();

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
        reportConfigWarning("No config block found for " + GetAppName());
    }

    for (auto &p : sParams) {

        string orig  = p;
        string line  = p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        bool handled = false;
        if ("BLOCK" == param) {
            rapidjson::Document d;
            if (d.Parse(value.c_str()).HasParseError()) {
                cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
                cerr << " in " << param << " at offset " << d.GetErrorOffset() << endl;
                std::abort();
            } else {
                unique_ptr<MuxBlockBase> tmp = MuxBlockBase::muxBlockFactory(d);
                if (tmp) {
                    blocks.push_back(move(tmp));
                } else {
                    cerr << "Invalid block; exiting" << endl;
                    std::abort();
                }
            }
            handled = true;
        }

        if(!handled) reportUnhandledConfigWarning(orig);

    }

    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables
void Mux::registerVariables() {
    AppCastingMOOSApp::RegisterVariables();
    for (auto &b: blocks) {
        b->subscribe(this);
    }
}

//------------------------------------------------------------
// Procedure: buildReport()
bool Mux::buildReport() {
    m_msgs << "============================================ \n";
    m_msgs << " pMux                                        \n";
    m_msgs << "============================================ \n";
    list<string> headers;
    list<string> values;
    // Assemble the headers & values as a unified list
    for (auto &b: blocks) {
        list<string> tmph = b->buildReportHeader();
        for (auto &h: tmph) headers.push_back(h);
        list<string> tmpv = b->buildReportLines();
        for (auto &v: tmpv) values.push_back(v);
    }
    // Assemble the response
    ACTable actab(headers.size());
    for (auto &h: headers) actab << h;
    actab.addHeaderLines();
    for (auto &v: values) actab << v;
    m_msgs << actab.getFormattedString();

    return(true);
}
