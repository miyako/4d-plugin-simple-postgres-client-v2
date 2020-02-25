/* --------------------------------------------------------------------------------
 #
 #  4DPlugin-Simple-PostgreSQL-Client.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : Simple PostgreSQL Client
 #	author : miyako
 #	2020/02/25
 #  
 # --------------------------------------------------------------------------------*/

#include "4DPlugin-Simple-PostgreSQL-Client.h"

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params) {
    
	try
	{
        switch(selector)
        {
			// --- Simple PostgreSQL Client
            
			case 1 :
				PQ_EXECUTE(params);
				break;

        }

	}
	catch(...)
	{

	}
}

#pragma mark -

void PQ_EXECUTE(PA_PluginParameters params) {
    
    sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
    PackagePtr pParams = (PackagePtr)params->fParameters;
    
    C_TEXT Param1_connectdb;/* connect */
    C_TEXT Param2_command;/* SQL */
    C_LONGINT Param4_format;
    
    Param1_connectdb.fromParamAtIndex(pParams, 1);
    Param2_command.fromParamAtIndex(pParams, 2);
    PA_CollectionRef Param3_params = PA_GetCollectionParameter(params, 3);
    Param4_format.fromParamAtIndex(pParams, 4);
    
    PA_ObjectRef returnValue = PA_CreateObject();
            
    CUTF8String connectdb, command, errorMessage;
    Param1_connectdb.copyUTF8String(&connectdb);
    
    PGconn *conn = PQconnectdb((const char *)connectdb.c_str());
    
    unsigned int paramCount = 0;
    
    if(Param3_params) {
        paramCount = PA_GetCollectionLength(Param3_params);
    }
    
    bool useParams = (paramCount > 1);
    
    if(PQstatus(conn) == CONNECTION_OK) {
        
        Param2_command.copyUTF8String(&command);
        
        PGresult *result;
        
        if(!useParams) {
            result = PQexecParams(conn,
                                  (const char *)command.c_str(),
                                  0,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  Param4_format.getIntValue());
        }else{
            
            std::vector<char *> paramValues;
            std::vector<int> paramLengths;
            
            std::vector<Oid> paramTypes;
            std::vector<int> paramFormats;
            
            std::vector<CUTF8String> paramStringValues;
            
            for(unsigned int i = 0; i < paramCount; ++i) {
                
                PA_Variable v = PA_GetCollectionElement(Param3_params, i);
                
                PA_VariableKind kind = PA_GetVariableKind(v);
                
                switch (kind) {
                    case eVK_Unistring:
                    {
                        PA_Unistring u = PA_GetStringVariable(v);
                        C_TEXT t;
                        t.setUTF16String((const PA_Unichar *)u.fString, u.fLength);
                        CUTF8String u8;
                        t.copyUTF8String(&u8);
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                        break;
                   
                    /* the code below are experimental an untested */
                        
                    case eVK_Real:
                    {
                        double r = PA_GetRealVariable(v);
                        int precision = 99;
                        std::vector<char> buf(32 + precision);
                        snprintf((char *)&buf[0], buf.size(), "%.*f", precision, r);
                        CUTF8String u8((const uint8_t *)&buf[0]);
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                        break;
                        
                    case eVK_Time:
                    {
                        PA_long32 t = PA_GetTimeVariable(v);
                        std::vector<char> buf(32);
                        snprintf((char *)&buf[0], buf.size(), "%d", t);
                        CUTF8String u8((const uint8_t *)&buf[0]);
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                        break;
                        
                    case eVK_Longint:
                    {
                        PA_long32 t = PA_GetLongintVariable(v);
                        std::vector<char> buf(32);
                        snprintf((char *)&buf[0], buf.size(), "%d", t);
                        CUTF8String u8((const uint8_t *)&buf[0]);
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                        break;
                        
                    case eVK_Null:
                    {
                        CUTF8String u8 = (const uint8_t *)"null";
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                        break;
                        
                    case eVK_Boolean:
                    {
                        char b = PA_GetBooleanVariable(v);
                        CUTF8String u8 = b ? (const uint8_t *)"true" : (const uint8_t *)"false";
                        paramStringValues.push_back(u8);
                        paramTypes.push_back(0);
                        paramFormats.push_back(0);
                        paramValues.push_back((char *)u8.c_str());
                        paramLengths.push_back((int)u8.size());
                    }
                            break;
                        
                    default:
                        break;
                }
  
            }
            
            result = PQexecParams(conn,
                                     (const char *)command.c_str(),
                                     paramCount-1,
                                     &paramTypes[0],
                                     &paramValues[0],
                                     &paramLengths[0],
                                     &paramFormats[0],
                                     Param4_format.getIntValue());
        }
        
        PA_CollectionRef resultRows = PA_CreateCollection();

        ob_set_s(returnValue, L"errorMessage", PQerrorMessage(conn));
        ob_set_s(returnValue, L"db", PQdb(conn));
        ob_set_s(returnValue, L"user", PQuser(conn));
        ob_set_s(returnValue, L"pass", PQpass(conn));
        ob_set_s(returnValue, L"host", PQhost(conn));
        //ob_set_s(returnValue, L"hostaddr", PQhostaddr(conn));
        ob_set_s(returnValue, L"port", PQport(conn));
        ob_set_s(returnValue, L"tty", PQtty(conn));
        ob_set_s(returnValue, L"options", PQoptions(conn));
        
        ob_set_n(returnValue, L"status", PQstatus(conn));
        ob_set_n(returnValue, L"transactionStatus", PQtransactionStatus(conn));
        ob_set_n(returnValue, L"protocolVersion", PQprotocolVersion(conn));
        ob_set_n(returnValue, L"socket", PQsocket(conn));
        ob_set_n(returnValue, L"backendPID", PQbackendPID(conn));
        ob_set_n(returnValue, L"clientEncoding", PQclientEncoding(conn));
        ob_set_n(returnValue, L"protocolVersion", PQprotocolVersion(conn));
        
        ob_set_b(returnValue, L"connectionNeedsPassword", PQconnectionNeedsPassword(conn));
        ob_set_b(returnValue, L"connectionUsedPassword", PQconnectionUsedPassword(conn));
        ob_set_b(returnValue, L"sslInUse", PQsslInUse(conn));
        //ob_set_b(returnValue, L"gssEncInUse", PQgssEncInUse(conn));
        
        switch (PQresultStatus(result))
        {
            case PGRES_EMPTY_QUERY:
            case PGRES_BAD_RESPONSE:
            case PGRES_FATAL_ERROR:
            case PGRES_NONFATAL_ERROR:
                
                break;
                
            case PGRES_TUPLES_OK:
            case PGRES_SINGLE_TUPLE:
            {
                int countRows = PQntuples(result);
                int countFields = PQnfields(result);
                
                for(unsigned int r = 0; r < countRows;++r)
                {
                    PA_ObjectRef resultRow = PA_CreateObject();
                    
                    for(unsigned int c = 0; c < countFields;++c)
                    {
                        char *fieldName = PQfname(result, c);
                        char *fieldValue = PQgetvalue(result, r, c);
                        int fieldIsNull = PQgetisnull(result, r, c);
                        int fieldLength = PQgetlength(result, r, c);
                        int fieldFormat = PQfformat(result, c);
                        
//                        Oid fieldType = PQftype(result, c);
//                        int fieldModifier = PQfmod(result, c);
                        
                        C_TEXT t;
                        t.setUTF8String((const uint8_t *)fieldName, (uint32_t)strlen(fieldName));
                        
                        CUTF16String u16;
                        t.copyUTF16String(&u16);
                        
                        if(fieldIsNull)
                        {
                            PA_Variable v = PA_CreateVariable(eVK_Null);
                            PA_Unistring key = PA_CreateUnistring((PA_Unichar *)u16.c_str());
                            
                            PA_SetObjectProperty(resultRow, &key, v);
                            
                            PA_DisposeUnistring(&key);
                            PA_ClearVariable(&v);
                            
                        }else{
                            switch (fieldFormat)
                            {
                                case 0:
                                {
                                    //zero indicates textual data representation
                                    ob_set_s(resultRow, fieldName, fieldValue);
                                }
                                    break;
                                    
                                default:
                                {
                                    //one indicates binary representation
                                    //other codes are reserved for future definition
                                    
                                    C_BLOB data;
                                    data.setBytes((const uint8_t *)fieldValue, fieldLength);
                                    
                                    C_TEXT t;
                                    data.toB64Text(&t);
                                    
                                    CUTF16String u16;
                                    t.copyUTF16String(&u16);
                                    
                                    PA_ObjectRef resultCell = PA_CreateObject();
                                    ob_set_a(resultCell, L"data", &u16);
                                    
                                    ob_set_o(resultRow, fieldName, resultCell);
                                }
                                    break;
                            }
                        }
                          
                    }/* c */
                    
                    PA_Variable v = PA_CreateVariable(eVK_Object);
                    PA_SetObjectVariable(&v, resultRow);
                    PA_SetCollectionElement(resultRows, r, v);
                    
                }/* r */
                
                ob_set_c(returnValue, L"values", resultRows);
                
                PQclear(result);
            }
                break;
                
            case PGRES_COMMAND_OK:
                PQclear(result);
                break;
                
            default:
                break;
        };
        
        PQfinish(conn);
        
    }

    PA_ReturnObject(params, returnValue);
}

