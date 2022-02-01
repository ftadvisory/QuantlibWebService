#ifndef VanillaSwapDto_hpp
#define VanillaSwapDto_hpp

#include "QuantlibBaseDto,hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include <ql/instruments/vanillaswap.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(SwapType, v_int32,
     VALUE(PAYER, QuantLib::Swap::Payer, "PAYER"),
     VALUE(RECEIVER, QuantLib::Swap::Receiver, "RECIEVER")
);

class VanillaSwapDto : public oatpp::DTO {
  
     DTO_INIT(VanillaSwapDto, DTO);

     DTO_FIELD(Int32, id);         // id 
     DTO_FIELD(Float32, notional);  // notional
     DTO_FIELD(Enum<SwapType>::AsNumber, type, "type");
     DTO_FIELD(Object<QuantlibCurrencyDto>, currency, "currency");
     DTO_FIELD(Int64, startDate);  // days since 12/31/1899 inclusie
     DTO_FIELD(Int64, mtyDate);    // days since 12/31/1899 inclusive
     DTO_FIELD(Float32, fixedRate); // percent
     DTO_FIELD(Object<QuantlibFrequencytDto>, fixedFrequency, "fixedFrequency");
     DTO_FIELD(Object<QuantlibDaycountTypeDto>, fixedDaycount, "fixedDaycount");
     DTO_FIELD(Object<QuantlibBusinessDayConventionDto>, fixedDayConvention, "fixedDayConvention");
     DTO_FIELD(Float32, floatSpread);  // percent
     DTO_FIELD(Object<QuantlibIndextDto>, floatIndexFamily, "floatIndexFamily");
     DTO_FIELD(Object<QuantlibFrequencytDto>, floatFrequency, "floatFrequency");
     DTO_FIELD(Object<QuantlibFrequencytDto>, floatTenor, "floatTenor");
     DTO_FIELD(Object<QuantlibDaycountTypeDto>, floatDaycount, "floatDaycount");
     DTO_FIELD(Object<QuantlibBusinessDayConventionDto>, floatDayConvention, "floatDayConvention");
     
     QuantLib::VanillaSwap::Type getType () {
          SwapType swapType = type;
          switch (swapType) {
               case SwapType::PAYER:
                    return QuantLib::VanillaSwap::Payer;
               case SwapType::RECEIVER:
                    return QuantLib::VanillaSwap::Receiver;
               default:
                    break;
          }
          throw ("VanillaSwapDto - Undefined type");
     }
     QuantLib::Currency  getCurrency () {return currency->getCurrency ();}
     QuantLib::Frequency getFrequency (bool fixedFlag) {
          return (fixedFlag) ? fixedFrequency->getFrequency () : floatFrequency->getFrequency ();
     }
     QuantLib::Period    getFloatTenor () {return QuantLib::Period (floatTenor->getFrequency ());}
     QuantLib::ext::shared_ptr<QuantLib::IborIndex> getFloatingRateIndex () {
          return floatIndexFamily->getIndex (currency->getCurrency (), getFloatTenor ());
     }
     QuantLib::ext::shared_ptr<QuantLib::DayCounter> getFixedDayCounter () {return fixedDaycount->getDayCounter ();}
     QuantLib::ext::shared_ptr<QuantLib::DayCounter> getFloatDayCounter () {return floatDaycount->getDayCounter ();}
     QuantLib::BusinessDayConvention getBusinessDayConvention (bool fixedFlag) {
          return (fixedFlag) ? fixedDayConvention->get () : floatDayConvention->get ();
     }
     QuantLib::ext::shared_ptr<QuantLib::Schedule> buildFixedSchedule (QuantLib::Calendar calendar) {
          return buildSchedule (calendar, true);
     }
     QuantLib::ext::shared_ptr<QuantLib::Schedule> buildFloatSchedule (QuantLib::Calendar calendar) {
          return buildSchedule (calendar, false);
     }
     QuantLib::ext::shared_ptr<QuantLib::Schedule> buildSchedule (QuantLib::Calendar calendar, bool fixedFlag) {
          QuantLib::BusinessDayConvention bdc = getBusinessDayConvention (fixedFlag);
          return QuantLib::ext::shared_ptr<QuantLib::Schedule> 
               (new QuantLib::Schedule (QuantLib::Date (startDate),
                                        QuantLib::Date (mtyDate),
                                        QuantLib::Period (getFrequency (fixedFlag)),
                                        calendar,
                                        bdc,
                                        bdc,
                                        QuantLib::DateGeneration::Forward,
                                        false));
     }
     QuantLib::ext::shared_ptr<QuantLib::VanillaSwap> buildSwap (QuantLib::Calendar calendar) {
		QuantLib::ext::shared_ptr<QuantLib::IborIndex> floatIndex = getFloatingRateIndex ();
          return QuantLib::ext::shared_ptr<QuantLib::VanillaSwap>
               (new QuantLib::VanillaSwap (getType (), 
                                           notional,
                                           *(buildFixedSchedule (calendar)), 
                                           fixedRate, 
                                           *(getFixedDayCounter ()),
                                           *(buildFloatSchedule (calendar)), 
                                           floatIndex, 
                                           floatSpread, 
                                           floatIndex->dayCounter()));
     }
};

#include OATPP_CODEGEN_END(DTO)

#endif /* VanillaSwapDto_hpp */
