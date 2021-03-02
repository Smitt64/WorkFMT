// -----------------------------------------------------------------------------
// 2011.11.21  Класс для работы с версиями
//
// Обобщенная форма версии: A.BB.CCC.DD.EE
//                          │  │  │   │  │
//                          │  │  │   │  └────
//                          │  │  │   └───────
//                          │  │  └───────────
//                          │  └──────────────
//                          └─────────────────
//
// Количество символов в элементе должно соответствовать шаблону, иначе будут
// проблемы с формированием имени словаря изменений. При этом номера, меньшие 
// обозначенного в шаблоне, дополняются лидирующими нулями.
// Т.о. максимальная длина строкового идентификатора составляет 14 символов.
#ifndef _RSVERSION_H_
#define _RSVERSION_H_

// -----------------------------------------------------------------------------
#define  dRSVER_UNUSED   -1    // Значение для неиспользуемых элементов

// -----------------------------------------------------------------------------
// Константы элементов версии
typedef enum TERSVersion
      {
       eRSVER_HI = 0,
       eRSVER_LOW,
       eRSVER_BUILD,
       eRSVER_PATCH,
       eRSVER_UPDATE
      } TERSVersion;

// -----------------------------------------------------------------------------
class CRSVersion
    {
     private:
       int  m_v_hi, m_v_low,     // Старший и младший номер версии
            m_v_build,           // Номер билда
            m_v_patch,           // Номер патча
            m_v_update;          // Номер обновления

       void Init(void);
       void Copy(const CRSVersion &o);

     public:
       CRSVersion();
       CRSVersion(const char *s);
       CRSVersion(const CRSVersion &o);
       CRSVersion(int v_hi, int v_low = dRSVER_UNUSED, int v_build = dRSVER_UNUSED, int v_patch = dRSVER_UNUSED, int v_update = dRSVER_UNUSED);
      ~CRSVersion();

       void Set(int v_hi = dRSVER_UNUSED, int v_low = dRSVER_UNUSED, int v_build = dRSVER_UNUSED, int v_patch = dRSVER_UNUSED, int v_update = dRSVER_UNUSED);
       void Get(int *v_hi = NULL, int *v_low = NULL, int *v_build = NULL, int *v_patch = NULL, int *v_update = NULL);

       bool FromString(const char *s);
       bool ToString(char *s);

       int  Cmp(const CRSVersion &r);

       CRSVersion &operator =  (const CRSVersion &r);
       int         operator [] (TERSVersion i);

       bool        operator == (const CRSVersion &r);
       bool        operator <  (const CRSVersion &r);
       bool        operator >  (const CRSVersion &r);
       bool        operator <= (const CRSVersion &r);
       bool        operator >= (const CRSVersion &r);
       bool        operator != (const CRSVersion &r);
    };

#endif // _VERSION_H_

/* <-- EOF --> */