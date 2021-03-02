#pragma once
//===================================================================
//             Автоматизированная банковская система RS-Bank
//                 Copyright c R-Style SoftLab 2004
//
//  Имя файла        : Al_CRsdCommand.hpp
//
//  Библиотека       : ALAPST.LIB
//
//  Описание         : Вспомогательные вещи для работы с RSD
//
//  Программист      : Орлов В.Н.
//
//  Создан           : 28.04.2004
//  Добавлена возможность получения результатов : 29.12.2014
//===================================================================
#ifndef _Al_CRsdCommand_unit
#define _Al_CRsdCommand_unit 1

#include<typeinfo>

// класс для упрощения передачи параметров при выполнении RSD-команд 

class  Al_CRsdCommand : public CRsdCommand
{
    class Tinfo 
    {
    public:
      void* var_in;         //указатель на переданную переменную
      int var_in_size;      //размер переданной переменной
      RSDBindParamIO_t dir; //направление вывода переменной
      void* var_temp;       //создаваемая временная переменная
      long  var_temp_size;  //длинна создаваемой временной переменной
      bool memoryAlocateAsArray; // Память выделена как для массива

      Tinfo(void* var_in = NULL,int var_in_size = 0,RSDBindParamIO_t dir = RSDBP_IN,void* var_temp = NULL,int   var_temp_size = 0, bool memoryAlocateAsArray = false)
      {
          this->var_in = var_in;
          this->var_in_size = var_in_size;
          this->dir = dir;
          this->var_temp = var_temp;
          this->var_temp_size =var_temp_size;
          this->memoryAlocateAsArray = memoryAlocateAsArray;
      }

    };

    RSDValType_t getRSDValType(int val)
            {
      return RSDPT_LONG;
            }

    RSDValType_t getRSDValType(int32 val)
            {
      return RSDPT_LONG;
            }

    RSDValType_t getRSDValType(int16 val)
            {
      return RSDPT_SHORT;
            }

    RSDValType_t getRSDValType(bdate val)
            {
      return RSDPT_DATE;
            }            

    RSDValType_t getRSDValType(btime val)
            {
      return RSDPT_TIME;
            }  

    RSDValType_t getRSDValType(lmoney val)
            {
      return RSDPT_DECIMAL;
            }              

    RSDValType_t getRSDValType(char val)
            {
      return RSDPT_CHAR;
            }

    RSDValType_t getRSDValType(double val)
            {
      return RSDPT_DOUBLE;
            }

    RSDValType_t getRSDValType(int64_t val)
            {
      return RSDPT_BIGINT;
            }

    TRsbList<Tinfo> lst_info;

   void add_parm( RSDValType_t type, void *val, long *len, RSDBindParamIO_t dir )
   {            
       addParam( "", type, val, len,*len,dir);
   }

   //дружественная функция позволяющая вызвать привантыные методы
   friend int friend_fun_SQLCmdExecute( Al_CRsdCommand& cmd, const char *pszMsg = NULL, btime *pbtm = NULL);

   int SQLCmdExecute(const char *pszMsg = NULL, btime *pbtm = NULL)
   {
       int stat = 0;
       stat = AL_SQLCmdExecute((CRsdCommand&)*this , pszMsg, pbtm );
       LPOSITION pos = lst_info.GetTailPosition();
       Tinfo info;
       while( pos )
       {
           info = lst_info.GetAt(pos);
           if (info.dir != RSDBP_IN)
           {
               memcpy(info.var_in,info.var_temp,info.var_in_size);
           }
           lst_info.GetPrev( pos );
       }
       return stat;
   }

public: 

    template <class Type_A> void add(Type_A a)
    { 
        RSDValType_t  RSDValType= getRSDValType(a);
        Tinfo info(&a,sizeof(a),RSDBP_IN,new Type_A(a),sizeof(Type_A));
        LPOSITION pos = lst_info.AddTail(info);
        add_parm( RSDValType, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );
    };

    //Для поддержки старой реализации необходимо преобразовывать bool к int
    void Al_CRsdCommand::add( bool val)
    {
        add(int(val));
    }

    void Al_CRsdCommand::add( const char *val)
    {
        int bufSizeForVal = strlen(val)+1;
        char *StrNew = new char[bufSizeForVal];
        if( StrNew )
        {
          strncpyz( StrNew, val, bufSizeForVal );
          Tinfo info(&val,0, RSDBP_IN, StrNew, bufSizeForVal, true);
          LPOSITION pos = lst_info.AddTail(info);
          add_parm( RSDPT_LPSTR, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );
        }
    }

    void Al_CRsdCommand::add(const std::string &str)
    {
        add((const char*)str.c_str());
    }

    //из-за енумов придется объявить явно
    void Al_CRsdCommand::add( int val )
    {    
         Tinfo info(&val,sizeof(val),RSDBP_IN,new int32(val),sizeof(int32));
         LPOSITION pos = lst_info.AddTail(info);
         add_parm( RSDPT_LONG, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );
    }


    template <class Type_A> void add( Type_A* val,RSDBindParamIO_t dir = RSDBP_IN)
    {
        RSDValType_t  RSDValType= getRSDValType(*val);
        int len = sizeof(Type_A);
        Tinfo info(val,len,dir,new Type_A(*val),sizeof(Type_A));
        LPOSITION pos = lst_info.AddTail(info);
        add_parm( RSDValType, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );
    };  
    
    //Строка или символ определяем по длине
    void Al_CRsdCommand::add(char *val,int len,RSDBindParamIO_t dir = RSDBP_IN)
    {
        Tinfo info(val,len,dir);
        RSDValType_t RSDValType = RSDPT_LPSTR;
        if (len == sizeof(char))
        {
           RSDValType = RSDPT_CHAR;
           info.var_temp = new char(*val);
           info.var_temp_size = sizeof(char);
        }
       else
        {
            char *StrNew = new char[len];
            info.memoryAlocateAsArray = true;
            strncpyz(StrNew,val,len);
            info.var_temp = StrNew;
            info.var_temp_size = len;
        }
        LPOSITION pos = lst_info.AddTail(info);
        add_parm( RSDValType, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );

    } 

    void Al_CRsdCommand::add(void *val,int len,RSDBindParamIO_t dir = RSDBP_IN)
    {
        Tinfo info(val,len,dir);
        RSDValType_t RSDValType = RSDPT_BINARY;
        char *StrNew = new char[len];
        info.memoryAlocateAsArray = true;
        memcpy(StrNew,val,len);
        info.var_temp = StrNew;
        info.var_temp_size = len;
        LPOSITION pos = lst_info.AddTail(info);
        add_parm( RSDValType, lst_info.GetAt(pos).var_temp, &lst_info.GetAt(pos).var_temp_size,lst_info.GetAt(pos).dir );
    } 

    Al_CRsdCommand::Al_CRsdCommand( CRsdConnection& con, const char *text = NULL, RSDCursorType_t CursorType = RSDVAL_FORWARD_ONLY ) : CRsdCommand( con, text, CursorType )
    {
    }
    Al_CRsdCommand::~Al_CRsdCommand()
    {
        LPOSITION pos = lst_info.GetTailPosition();
        while( pos )
        {
            if( lst_info.GetAt( pos ).memoryAlocateAsArray )
              delete [] lst_info.GetAt( pos ).var_temp;
            else
              delete lst_info.GetAt( pos ).var_temp;

            lst_info.GetPrev( pos );
        }
    }
};
#endif // _Al_CRsdCommand_unit
