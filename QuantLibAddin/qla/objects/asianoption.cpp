
/*
 Copyright (C) 2005 Eric Ehlers

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <qla/objects/asianoption.hpp>
#include <qla/objects/optionutils.hpp>

namespace QuantLibAddin {

    QuantLib::Average::Type IDtoAverageType(const std::string &typeAverage) {
        std::string idUpper = QuantLib::StringFormatter::toUppercase(typeAverage);
        if (idUpper.compare("A") ==0)
            return QuantLib::Average::Arithmetic;
        else if (idUpper.compare("G") == 0)
            return QuantLib::Average::Geometric;
        else
            QL_FAIL("IDtoAverageType: unrecognized typeID: " + typeAverage);
    }

    ContinuousAveragingAsianOption::ContinuousAveragingAsianOption(
            const boost::shared_ptr<StochasticProcess> &stochasticProcess,
            const std::string &typeAverage,
            const std::string &typeOption,
            const std::string &typePayoff,
            const float &strike,
            const std::string &typeExercise,
            const long &exerciseDate,
            const long &settlementDate,
            const std::string &typeEngine,
            const long &timeSteps) {
        QuantLib::Average::Type averageType =
            IDtoAverageType(typeAverage);
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff =
            IDtoPayoff(typeOption, typePayoff, strike);
        boost::shared_ptr<QuantLib::Exercise> exercise = 
            IDtoExercise(typeExercise, QuantLib::Date(exerciseDate), 
                QuantLib::Date(settlementDate));
        boost::shared_ptr<QuantLib::PricingEngine> pricingEngine =
            IDtoEngine(typeEngine, timeSteps);
        const boost::shared_ptr<QuantLib::BlackScholesProcess> stochasticProcessQL = 
            boost::static_pointer_cast<QuantLib::BlackScholesProcess>
            (stochasticProcess->getReference());
        continuousAveragingAsianOption_ = 
            boost::shared_ptr<QuantLib::ContinuousAveragingAsianOption>(
                new QuantLib::ContinuousAveragingAsianOption(
                    averageType,
                    stochasticProcessQL, 
                    payoff, 
                    exercise, 
                    pricingEngine));
        ObjHandler::any_ptr any_npv(
            new boost::any(continuousAveragingAsianOption_->NPV()));
        ObjHandler::any_ptr any_engine(
            new boost::any(std::string(typeEngine)));
        ObjHandler::ObjectProperty prop_npv(FIELD_NPV, any_npv);
        ObjHandler::ObjectProperty prop_engine(FIELD_ENGINE, any_engine);
        properties_.push_back(prop_npv);
        properties_.push_back(prop_engine);
    }

    DiscreteAveragingAsianOption::DiscreteAveragingAsianOption(
            const boost::shared_ptr<StochasticProcess> &stochasticProcess,
            const std::string &typeAverage,
            const float &runningAccumulator,
            const long &pastFixings,
            const std::vector<long> &fixingDates,
            const std::string &typeOption,
            const std::string &typePayoff,
            const float &strike,
            const std::string &typeExercise,
            const long &exerciseDate,
            const long &settlementDate,
            const std::string &typeEngine,
            const long &timeSteps) {
        QuantLib::Average::Type averageType =
            IDtoAverageType(typeAverage);
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff =
            IDtoPayoff(typeOption, typePayoff, strike);
        boost::shared_ptr<QuantLib::Exercise> exercise = 
            IDtoExercise(typeExercise, QuantLib::Date(exerciseDate), 
                QuantLib::Date(settlementDate));
        boost::shared_ptr<QuantLib::PricingEngine> pricingEngine =
            IDtoEngine(typeEngine, timeSteps);
        const boost::shared_ptr<QuantLib::BlackScholesProcess> stochasticProcessQL = 
            boost::static_pointer_cast<QuantLib::BlackScholesProcess>
            (stochasticProcess->getReference());
        std::vector<QuantLib::Date> fixingDatesQL;
        std::vector<long>::const_iterator i;
        for (i = fixingDates.begin(); i != fixingDates.end(); i++)
            fixingDatesQL.push_back(QuantLib::Date(*i));
        discreteAveragingAsianOption_ = 
            boost::shared_ptr<QuantLib::DiscreteAveragingAsianOption>(
                new QuantLib::DiscreteAveragingAsianOption(
                    averageType,
                    runningAccumulator,
                    pastFixings,
                    fixingDatesQL,
                    stochasticProcessQL, 
                    payoff, 
                    exercise, 
                    pricingEngine));
        ObjHandler::any_ptr any_npv(
            new boost::any(discreteAveragingAsianOption_->NPV()));
        ObjHandler::any_ptr any_engine(
            new boost::any(std::string(typeEngine)));
        ObjHandler::ObjectProperty prop_npv(FIELD_NPV, any_npv);
        ObjHandler::ObjectProperty prop_engine(FIELD_ENGINE, any_engine);
        properties_.push_back(prop_npv);
        properties_.push_back(prop_engine);
    }

}

