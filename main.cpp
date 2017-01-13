#define _USE_MATH_DEFINES
#include <iostream>
#include "HullWhite.h"
#include <string>
#include "FunctionalUtilities.h"
#include "SimulateNorm.h"
#include "Histogram.h" //bins data
#include "document.h" //rapidjson
#include "writer.h" //rapidjson
#include "stringbuffer.h" //rapidjson
#define BOND 0
#define EURODOLLARFUTURE 1
#define COUPONBOND 2
#define BONDCALL 3
#define BONDPUT 4
#define COUPONBONDCALL 5
#define COUPONBONDPUT 6
#define CAPLET 7
#define SWAP 8 
#define SWAPTION 9
#define AMERICANSWAPTION 10

auto square=[](const auto& val){
    return val*val;
};
auto yield=[&](const auto& t, const auto& currRate, const auto& a, const auto& b, const auto& sig){
    auto at=(1-exp(-a*t))/a;
    auto ct=(b-sig*sig/(2*a*a))*(at-t)-sig*sig*at*at/(4*a);
    return (at*currRate-ct);
};
auto forward=[&](const auto& t, const auto& currRate, const auto& a, const auto& b, const auto& sig){
    return b+exp(-a*t)*(currRate-b)-(sig*sig/(2*a*a))*square(1-exp(-a*t));
};

int main(int argc, char* argv[]){
    double Maturity=1.0;
    const double daysInYear=365;
    double t=10.0/daysInYear;
    double Strike=.04;
    double Tenor=.25;
    double UnderlyingMaturity=1.25;
    int type=0;
    double a=.3;
    double b=.04;
    double sigma=.2;
    double r0=.02;
    int n=10000;
    SimulateNorm rNorm;
    if(argc>1){
        rapidjson::Document parms;
        parms.Parse(argv[1]);//yield data
        if(parms.FindMember("T")!=parms.MemberEnd()){
            Maturity=parms["T"].GetDouble();
        }
        if(parms.FindMember("t")!=parms.MemberEnd()){
            t=parms["t"].GetInt()/daysInYear;
        }
        if(parms.FindMember("k")!=parms.MemberEnd()){
            Strike=parms["k"].GetDouble();
        }
        if(parms.FindMember("delta")!=parms.MemberEnd()){
            Tenor=parms["delta"].GetDouble();
        }
        if(parms.FindMember("Tm")!=parms.MemberEnd()){
            UnderlyingMaturity=parms["Tm"].GetDouble();
        }
        if(parms.FindMember("asset")!=parms.MemberEnd()){
            type=parms["asset"].GetInt();
        }
        if(parms.FindMember("a")!=parms.MemberEnd()){
            a=parms["a"].GetDouble();
        }
        if(parms.FindMember("b")!=parms.MemberEnd()){
            b=parms["b"].GetDouble();
        }
        if(parms.FindMember("sigma")!=parms.MemberEnd()){
            sigma=parms["sigma"].GetDouble();
        }
        if(parms.FindMember("r0")!=parms.MemberEnd()){
            r0=parms["r0"].GetDouble();
        }
        if(parms.FindMember("n")!=parms.MemberEnd()){
            n=parms["n"].GetDouble();
        }
    }
    
    auto getType=[&](const auto& rate){
        auto tmpYield=[&](const auto& t){
            return yield(t, rate, a, b, sigma);
        };
        auto tmpForward=[&](const auto& t){
            return forward(t, rate, a, b, sigma);
        };
        switch(type){
            case BOND:
                return hullwhite::Bond_Price(rate, a, sigma, t, Maturity, tmpYield, tmpForward);
            case EURODOLLARFUTURE:
                return hullwhite::EuroDollarFuture(rate, a, sigma, t, Maturity, Tenor, tmpYield, tmpForward);
            case BONDCALL:
                return hullwhite::Bond_Call(rate, a, sigma, t, Maturity, UnderlyingMaturity, Strike, tmpYield, tmpForward);
            case BONDPUT:
                return hullwhite::Bond_Put(rate, a, sigma, t, Maturity, UnderlyingMaturity, Strike, tmpYield, tmpForward);
            case CAPLET:
                return hullwhite::Caplet(rate, a, sigma, t, Maturity, Tenor, Strike, tmpYield, tmpForward);
            case SWAP:
                return hullwhite::Swap_Price(rate, a, sigma, t, Maturity, Tenor, Strike, tmpYield, tmpForward);//swap rate is strike here...
            case SWAPTION:
                return hullwhite::Swaption(rate, a, sigma, Strike, t, UnderlyingMaturity, Maturity, Tenor, tmpYield, tmpForward);
            case AMERICANSWAPTION:
                return hullwhite::AmericanSwaption(rate, a, sigma, Strike, t, UnderlyingMaturity, Maturity, Tenor, tmpYield, tmpForward);
        }
    };
    double min=500; //purposely out of order because actual min and max are found within the function
    double max=-1;
    auto sendToCall=[&](const auto& val){
        std::cout<<val;
    };
    binAndSend(sendToCall,min, max, futilities::for_each_parallel(0, n, [&](const auto& index){
        return getType(hullwhite::generateVasicek(r0, a, b, sigma, t, rNorm.getNorm()));
    })); //send histogram to node
}
