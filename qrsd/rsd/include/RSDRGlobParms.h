#ifndef RSDRGLOBPARMS_H
#define RSDRGLOBPARMS_H

#ifdef RSDC_EXPORTS
#define RSDDLL __declspec (dllexport) 
#else
#define RSDDLL __declspec (dllimport) 
#endif

class RSDDLL CRSDRGlobParms
{
public:
   static CRSDRGlobParms& inst()
   {
      static CRSDRGlobParms self;
      return self;
   }

   //parms
   bool repairUpdatedCacheFieldsNullData_;

private:
   CRSDRGlobParms() :
       repairUpdatedCacheFieldsNullData_(false)
   {}

   CRSDRGlobParms(const CRSDRGlobParms &t) {}
   CRSDRGlobParms& operator=(const CRSDRGlobParms &t) { return *this; }
};


#endif