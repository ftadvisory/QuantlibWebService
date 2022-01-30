#include "SwapController.hpp"

#include <ql/qldefines.hpp>
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/vanillaswap.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/indexes/ibor/usdlibor.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/simpledaycounter.hpp>
#include <ql/time/schedule.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/cashflows/cashflowvectors.hpp>
#include <ql/utilities/dataformatters.hpp>
#include <ql/cashflows/cashflows.hpp>
#include <ql/currencies/europe.hpp>
#include <ql/functional.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/time/date.hpp>

#include <iostream>

using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    ThreadKey sessionId() { return {}; }

}
#endif

void SwapController::createSwap (Object<VanillaSwapDto> dto) {
	try {
        Calendar calendar 					    = TARGET();
        BusinessDayConvention fixConvention     = Unadjusted;
        BusinessDayConvention floatConvention   = Unadjusted;
		Date startDate                          = Date (dto->startDate);
		Date mtyDate                            = Date (dto->mtyDate);
		ext::shared_ptr<Quote> flatRate(new SimpleQuote(dto->fixedRate));
		ext::shared_ptr<DayCounter> floatDaycount = dto->getFloatDayCounter();
        Handle<YieldTermStructure> rhTermStructure (ext::make_shared<FlatForward> (startDate, Handle<Quote> (flatRate),*floatDaycount)); 
		ext::shared_ptr<IborIndex> floatIndex   = dto->getFloatingRateIndex (rhTermStructure);
		Schedule fixedSchedule (startDate, 
								mtyDate, 
								Period(dto->getFixedFrequency ()),
								calendar,
								fixConvention,
								fixConvention,
								DateGeneration::Forward,
								false);
		Schedule floatSchedule (startDate, 
								mtyDate, 
								Period(dto->getFloatFrequency ()),
								calendar,
								floatConvention,
								floatConvention,
								DateGeneration::Forward,
								false);                                							
        ext::shared_ptr<VanillaSwap> swap (
			new VanillaSwap (dto->getType (),
                             dto->notional,
							 fixedSchedule, dto->fixedRate, *(dto->getFixedDayCounter ()),
                             floatSchedule, floatIndex, dto->floatSpread, floatIndex->dayCounter()));
		std::cout << "SwapController swap start date: " << swap->startDate () << std::endl;
		std::cout << "SwapController swap end date: " << swap->maturityDate () << std::endl;
		std::cout << "SwapController fixed rate: " << swap->fixedRate () << std::endl;
		std::cout <<  "SwapController float tenor from dto ... input: " <<  static_cast<int>(*dto->floatTenor)
				  << " output: " << dto->getFloatTenor () << std::endl;
		std::cout << "SwapController ibor index: " << swap->iborIndex ()->name () << std::endl;

	} catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error" << std::endl;
    }
}