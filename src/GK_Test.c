/*
 * GK_Test.c
 *
 * Copyright (c) 2002, International Business Machines
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE COMMON PUBLIC LICENSE 
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Common Public License from
 * http://oss.software.ibm.com/developerworks/opensource/license-cpl.html
 *
 * Author: George Varghese<georgooty@gmail.com>      
 * Contributors:
 *
 * Interface Type : Common Manageability Programming Interface ( CMPI )
 *
 * Description: 
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include <ind_helper.h>

#include "SettingDB.h"



CMPIBroker * _broker;

/* ---------------------------------------------------------------------------*/
/* private declarations                                                       */

#ifdef CMPI_VER_100
#define GK_TestSetInstance GK_TestModifyInstance 
#endif



static char * _ClassName     = "GK_Test";
static char * _InstanceID     = "GK:Test";

/* private methods                                                            */
/*                                                                            */

/* - method to create a CMPIObjectPath of this class                          */
static CMPIObjectPath * _makePath_GK_Test( CMPIObjectPath * cop, CMPIStatus * rc);

/* - method to create a CMPIInstance of this class                            */
static CMPIInstance * _makeInst_GK_Test( CMPIObjectPath * cop, CMPIStatus * rc);

/* ---------------------------------------------------------------------------*/


/* ---------------------------------------------------------------------------*/
/*                      Instance Provider Interface                           */
/* ---------------------------------------------------------------------------*/

/*
The CMPIInstanceMIFTcleanup() function shall perform any necessary cleanup operation prior to the unloading of the library of which this MI group is part.
This function is called prior to the unloading of the provider.
Parameters:
mi - The mi argument is a pointer to a CMPIInstanceMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 

Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now. CMPI_RC_NEVER_UNLOAD Operation successful - never unload.o
*/
CMPIStatus GK_TestCleanup( CMPIInstanceMI * mi, 
           CMPIContext * ctx) {
  CMReturn(CMPI_RC_OK);
}

/*
Enumerate ObjectPaths of Instances serviced by this provider.
Parameters:
mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
ref - ObjectPath containing namespace and classname components. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful.
CMPI_RC_ERR_FAILED Unspecific error occurred. CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. 
CMPI_RC_ERR_NOT_FOUND Instance not found.
*/
CMPIStatus GK_TestEnumInstanceNames( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * ref) { 
	CMPIObjectPath * op = NULL;
	CMPIStatus       rc = {CMPI_RC_OK, NULL};
	
	 

	op = _makePath_GK_Test( ref, &rc );

	if( op == NULL) 	{

		CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_FAILED,
						"Couldn't build objectpath");    
		goto exit;
	}
	
	CMReturnObjectPath( rslt, op );
    CMReturnDone( rslt );
	
	exit:
	return rc;
}

/*
Enumerate the Instances serviced by this provider.

Parameters:
mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
ref - ObjectPath containing namespace and classname components. 
properties - If not NULL, the members of the array define one or more Property names. Each returned Object MUST NOT include elements for any Properties missing from this list. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_NOT_FOUND Instance not found.
*/
CMPIStatus GK_TestEnumInstances( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * ref, 
           char ** properties) { 
  CMPIStatus rc = {CMPI_RC_OK, NULL};
  const CMPIInstance * ci = NULL;  

  ci = _makeInst_GK_Test( ref, &rc );

  if( ci == NULL) {
    CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_FAILED,
                                                 "Couldn't build instance");
	goto exit;
  }
  
  CMReturnInstance( rslt, ci );
  CMReturnDone( rslt );

  exit:
  return rc;
}

/* 
Get the Instances defined by <op>.

Parameters:
mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
cop - ObjectPath containing namespace, classname and key components. 

properties - If not NULL, the members of the array define one or more Property names. Each returned Object MUST NOT include elements for any Properties missing from this list. 

Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_NOT_FOUND Instance not found.
*/
CMPIStatus GK_TestGetInstance( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * cop, 
           char ** properties) {
	const CMPIInstance * ci = NULL;
	CMPIStatus     rc = {CMPI_RC_OK, NULL};

	CMPIString         * instanceId  = NULL;
	printf("--- %s CMPI GetInstance() called",_ClassName);

	instanceId  = CMGetKey( cop, "InstanceID", &rc).value.string;
	if( instanceId  == NULL ) {    
		CMSetStatusWithChars( _broker, &rc, 
					CMPI_RC_ERR_NOT_FOUND, "Could not get InstanceID." ); 
   
   goto exit;
  }

   if ( strcmp(CMGetCharPtr(instanceId),_InstanceID) != 0){
      CMSetStatusWithChars( _broker, &rc,
                            CMPI_RC_ERR_NOT_FOUND, "This class name does not exist (wrong InstanceID)." );

		goto exit;
	}

  


	ci = _makeInst_GK_Test(cop,  &rc );

	if( ci == NULL ){

		CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_FAILED,
								"Couldn't build instance");
		goto exit;
	}


	CMReturnInstance( rslt, ci );
	CMReturnDone(rslt);

	exit:
	
  return rc;
}

/* 
Create Instance from <inst> using <op> as reference.

Parameters:

mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
op - ObjectPath containing namespace, classname and key components. 
inst - The Instance. 

Returns:

Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. 
CMPI_RC_ERR_FAILED Unspecific error occurred. CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ALREADY_EXISTS Instance already exists.
*/
CMPIStatus GK_TestCreateInstance( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * cop, 
           CMPIInstance * ci) {
  CMPIStatus rc = {CMPI_RC_OK, NULL};

  
  CMSetStatusWithChars( _broker, &rc, 
			CMPI_RC_ERR_NOT_SUPPORTED, "NOT_SUPPORTED" ); 
  return rc;
}

/* 
Replace an existing Instance from <inst> using <op> as reference.
Parameters:
mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
cop - ObjectPath containing namespace, classname and key components. 
ci - The Instance. 
properties - If not NULL, the members of the array define one or more Property names.
The process MUST NOT replace elements for any Properties missing from this list. If NULL all properties will be replaced. 
Returns:
Function return status.
*/
CMPIStatus GK_TestSetInstance( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * cop,
           CMPIInstance * ci, 
           char **properties) {
  CMPIStatus rc = {CMPI_RC_OK, NULL};

  
  
  CMSetStatusWithChars( _broker, &rc, 
			CMPI_RC_ERR_NOT_SUPPORTED, "NOT_SUPPORTED" ); 
  return rc;
}

/* 
Delete an existing Instance defined by <op>.
Parameters:
mi - Provider this pointer. 
ctx - Invocation Context. 
rslt - Result data container. 
op - ObjectPath containing namespace, classname and key components. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_NOT_FOUND Instance not found.
*/
CMPIStatus GK_TestDeleteInstance( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * cop) {
  CMPIStatus rc = {CMPI_RC_OK, NULL}; 

  

  CMSetStatusWithChars( _broker, &rc, 
			CMPI_RC_ERR_NOT_SUPPORTED, "NOT_SUPPORTED" ); 
  return rc;
}

/* 
Query the enumeration of instances of the class (and subclasses) defined by <op> using <query> expression.
Parameters:
mi - Provider this pointer. 
ctx - Context object 
rslt - Result data container. 
op - ObjectPath containing namespace and classname components. 
query - Query expression 
lang - Query language 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. 
CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED Query language not supported. CMPI_RC_ERR_INVALID_QUERY Invalid query
*/
CMPIStatus GK_TestExecQuery( CMPIInstanceMI * mi, 
           CMPIContext * ctx, 
           CMPIResult * rslt, 
           CMPIObjectPath * ref, 
           char * lang, 
           char * query) {
  CMPIStatus rc = {CMPI_RC_OK, NULL};

  

  CMSetStatusWithChars( _broker, &rc, 
			CMPI_RC_ERR_NOT_SUPPORTED, "NOT_SUPPORTED" ); 
  return rc;
}



/* ---------------------------------------------------------------------------*/
/*                        Method Provider Interface                           */
/* ---------------------------------------------------------------------------*/

/*
The CMPIMethodMIFTcleanup() function shall perform any necessary cleanup operation prior to the unloading of the library of which this MI group is part. 
This function is called prior to the unloading of the provider.
Parameters:
mi - The mi argument is a pointer to a CMPIMethodMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
terminating - When true, the terminating argument indicates that the MB is in the process of terminating and that cleanup must be done. 
When set to false, the MI may respond with CMPI_IRC_DO_NOT_UNLOAD, or CMPI_IRC_NEVER_UNLOAD, indicating that unload will interfere with current MI processing. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now. CMPI_RC_NEVER_UNLOAD Operation successful - never unload.
*/
CMPIStatus GK_TestMethodCleanup( CMPIMethodMI * mi, 
           CMPIContext * ctx) {
  CMReturn(CMPI_RC_OK);
}


/* 
Invoke a named, extrinsic method of an Instance defined by the <op> parameter.
Parameters:
mi - Provider this pointer. 
ctx - Invocation Context 
rslt - Result data container. 
ref - ObjectPath containing namespace, classname and key components. 
methodName - Method name 
in - Input parameters. 
out - Output parameters. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized.
CMPI_RC_ERR_NOT_FOUND Instance not found. CMPI_RC_ERR_METHOD_NOT_AVAILABLE Method not available. CMPI_RC_ERR_METHOD_NOT_FOUND Method not found.
*/
CMPIStatus GK_TestInvokeMethod( CMPIMethodMI * mi,
           CMPIContext * ctx,
           CMPIResult * rslt,
           CMPIObjectPath * ref,
           char * methodName,
           CMPIArgs * in,
           CMPIArgs * out) {
  CMPIString * class = NULL; 
  CMPIStatus   rc    = {CMPI_RC_OK, NULL};
  CMPIData     incmd;
  CMPIValue    valrc; 

  int retVal;
  const char* parms;
  CMPIString         * instanceId  = NULL;

  

  valrc.uint32 = 2; /*FAIL*/
  /*Obtain and validate classname*/
  class = CMGetClassName(ref, &rc);
  if( strcasecmp(CMGetCharPtr(class), _ClassName) != 0)
	goto exit;
  /*Obtain and validate keys*/
  instanceId  = CMGetKey( ref, "InstanceID", &rc).value.string;
  if( instanceId  == NULL ) {    
    CMSetStatusWithChars( _broker, &rc, 
			  CMPI_RC_ERR_NOT_FOUND, "Invalid selector(Could not get InstanceID of instance.)" ); 
  
   goto exit;
  }

   if ( strcmp(CMGetCharPtr(instanceId),_InstanceID) != 0){
      CMSetStatusWithChars( _broker, &rc,
                            CMPI_RC_ERR_NOT_FOUND, "This class name does not exist (wrong InstanceID)." );

      goto exit;
  }

   if( strcasecmp("SetSetting",methodName) == 0 ) {  

		incmd = CMGetArg( in, "Setting", &rc); 
		if (rc.rc != CMPI_RC_OK) {     
			CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_INVALID_PARAMETER, "Required argument Setting missing.");
			goto exit; 
		}  
		//check Enabled
		if (incmd.type != CMPI_string) 	{     
			CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_INVALID_PARAMETER, "Invalid type of parameter.");
			goto exit; 
		}   

		parms = CMGetCharPtr(incmd.value.string);   
		if(strlen(parms) < 1)	{
			CMSetStatusWithChars(_broker, &rc,
           							CMPI_RC_ERR_INVALID_PARAMETER, "Invalid parameter. Value is missing.");		  
			goto exit;

		}

	
		retVal =  UpdateSetting(parms);
		if(retVal == -1){
			CMSetStatusWithChars(_broker, &rc, CMPI_RC_ERR_FAILED, "Failed to update");	       
			goto exit;
		}   

  }
  else {
    CMSetStatusWithChars( _broker, &rc, 
			  CMPI_RC_ERR_NOT_SUPPORTED, methodName ); 
  }

   //method is successfull
  valrc.uint32 = 0;  


exit:
   CMReturnData( rslt, &valrc, CMPI_uint32); 
   CMReturnDone( rslt );


  return rc;
}



/* ---------------------------------------------------------------------------*/
/*                              private methods                               */
/* ---------------------------------------------------------------------------*/


/* ---------- method to create a CMPIObjectPath of this class ----------------*/

static CMPIObjectPath * _makePath_GK_Test( CMPIObjectPath * ref,
				   CMPIStatus * rc) {
  CMPIObjectPath * op = NULL;


  op = CMNewObjectPath( _broker, CMGetCharPtr(CMGetNameSpace(ref,rc)), 
			_ClassName, rc );
  if( CMIsNullObject(op) ) { 
    CMSetStatusWithChars( _broker, rc, 
			  CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed." ); 
    goto exit; 
  }


 
  CMAddKey( op, "InstanceID",_InstanceID, CMPI_chars);
    
 exit:
  return op;                
}

/* ----------- method to create a CMPIInstance of this class ----------------*/

static CMPIInstance * _makeInst_GK_Test( CMPIObjectPath * ref,
				 CMPIStatus * rc) {
  CMPIObjectPath * op     = NULL;
  CMPIInstance   * ci     = NULL;
   char regVal[25] = "";
  char instance_name[100] = {0};
  int retVal= -1;
  CMPIValue       val;
  

  op = CMNewObjectPath( _broker, CMGetCharPtr(CMGetNameSpace(ref,rc)), 
			_ClassName, rc );
  if( CMIsNullObject(op) ) { 
    CMSetStatusWithChars( _broker, rc, 
			  CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed." ); 
    goto exit; 
  }

  ci = CMNewInstance( _broker, op, rc);
  if( CMIsNullObject(ci) ) { 
    CMSetStatusWithChars( _broker, rc, 
			  CMPI_RC_ERR_FAILED, "Create CMPIInstance failed." ); 
    goto exit; 
  }
    
   sprintf(instance_name, "%s-Test", "localhost");

  retVal =  GetRegisterValue(1,regVal);
  if(retVal== 0){
	   
		val.boolean = atoi(regVal);
		CMSetProperty(ci, "Enabled", &val, CMPI_boolean);
		
		val.uint16 = atoi(regVal);
		CMSetProperty(ci, "Setting", &val, CMPI_uint16);
  }

  CMSetProperty( ci, "Caption", _ClassName, CMPI_chars);
  CMSetProperty( ci, "ElementName", _ClassName, CMPI_chars);
  CMSetProperty( ci, "InstanceID", _InstanceID, CMPI_chars);
  CMSetProperty( ci, "Description", "A class derived from Logical Element that represents the single node container of the GK.", CMPI_chars);
  CMSetProperty( ci, "Name", instance_name, CMPI_chars);

 
		
	


 exit:
  return ci;
}



/* ---------------------------------------------------------------------------*/
/*                       Indication Provider Interface                        */
/* ---------------------------------------------------------------------------*/
#define INDCLASSNAME      "GK_TestModification"//CIM_InstModification
#define INDNAMESPACE      "root/cimv2"
#define IND_NUMBER_OF_DYNAMIC_PROPERTIES  5

static int ind_inited  = 0;
static int ind_enabled = 0;
static int ind_new     = 0;

static int ind_OperationalStatus = 2;

static CMPIInstance *sourceInstance   = NULL;
static CMPIInstance *previousInstance = NULL;

IndErrorT GK_Test_Indication_IndicationIdentifier(CMPIData *v)
{
  CMPIString *str;
  CMPIStatus rc;

  printf("--- %s CIM_Indication_IndicationIdentifier() called\n",_ClassName);

  str = CMNewString(_broker,INDCLASSNAME,&rc);
  v->type = CMPI_string;
  v->value.string = str;
  v->state = CMPI_goodValue;
  printf("--- %s CIM_Indication_IndicationIdentifier() leave \n",_ClassName);
  return IND_OK;
}

IndErrorT GK_Test_Indication_CorrelatedIndications(CMPIData *v)
{
  CMPIArray *a;
  CMPIStatus rc;
  
  printf("--- %s CIM_Indication_CorrelatedIndications() called\n",_ClassName);
  
  a = CMNewArray(_broker, 0, CMPI_string, &rc);
  v->type= CMPI_stringA;
  v->value.array = a;
  v->state = CMPI_goodValue;
    printf("--- %s CIM_Indication_CorrelatedIndications() leave\n",_ClassName);

  return IND_OK;
}

IndErrorT GK_Test_Indication_IndicationTime(CMPIData *v)
{
  CMPIStatus rc;
  CMPIDateTime *localTime = NULL;

  printf("--- %s CIM_Indication_IndicationTime() called\n",_ClassName);

  localTime = CMNewDateTime(_broker, &rc);
  v->type=CMPI_dateTime;
  v->value.dateTime = localTime;
  v->state = CMPI_goodValue;
  printf("--- %s CIM_Indication_IndicationTime() leave\n",_ClassName);

  return IND_OK;
}

IndErrorT GK_Test_InstModification_SourceInstance(CMPIData *v)
{
  CMPIStatus rc;

  printf("--- %s CIM_InstModification_SourceInstance() called\n",_ClassName);

  v->type = CMPI_instance; 
  v->value.inst = CMClone(sourceInstance,&rc);
  v->state = CMPI_goodValue;

  printf("--- %s CIM_InstModification_SourceInstance() leave\n",_ClassName);

  return IND_OK;
}

IndErrorT GK_Test_InstModification_PreviousInstance(CMPIData *v)
{
  CMPIStatus rc;

  printf("--- %s CIM_InstModification_PreviousInstance() called\n",_ClassName);

  v->type = CMPI_instance; 
  v->value.inst = CMClone(previousInstance,&rc);
  v->state = CMPI_goodValue;

  printf("--- %s CIM_InstModification_PreviousInstance() leave\n",_ClassName);

  return IND_OK;
}

int check_GKTestStatus() {
	
	int retVal;
	 char regVal[25] = "";
	retVal = GetRegisterValue(1,regVal);
   
	if(retVal != 0){   
		printf("Failed to get ColorSpace information from database." ); 
		return 0; 
	}

	ind_OperationalStatus = atoi(regVal);
	return 1;
}

IndErrorT check(CMPIData *v)
{
	CMPIObjectPath *cop = NULL;
	CMPIStatus      rc;

	printf("--- %s check() called\n",_ClassName);
	
	/* call function to check OperationalStatus */
	ind_new = check_GKTestStatus();
	if(ind_new!=0) 
		printf("--- %s check(): Event occured\n",_ClassName);

	if(previousInstance) {
		CMRelease(previousInstance);
		previousInstance=NULL; 
	}
	if(sourceInstance) {
		previousInstance = CMClone(sourceInstance,&rc);
		CMRelease(sourceInstance);
		sourceInstance=NULL;
	}

	cop = CMNewObjectPath( _broker,INDNAMESPACE,_ClassName,&rc);
	 if( CMIsNullObject(cop) ) { 
		printf("Create CMPIObjectPath failed.\n" ); 
		return; 
	}
	
	sourceInstance = CMClone(_makeInst_GK_Test(cop, &rc ),&rc);

	v->state = CMPI_goodValue;
	v->type = CMPI_uint16;
	v->value.uint16 = ind_OperationalStatus;

	ind_new = 0;
	printf("--- %s check() exited\n",_ClassName);
	return IND_OK;
}


/* The list of property names for the functions */
static const char *GK_TestModification_DYNAMIC_PROPERTIES[] =
  {"IndicationIdentifier",
   "CorrelatedIndications",
   "IndicationTime",
   "SourceInstance",
   "PreviousInstance"
  };

/* The list of property functions  */
static IndErrorT (* GK_TestModification_DYNAMIC_FUNCTIONS[])(CMPIData *v) =
  {GK_Test_Indication_IndicationIdentifier,
   GK_Test_Indication_CorrelatedIndications,
   GK_Test_Indication_IndicationTime,
   GK_Test_InstModification_SourceInstance,
   GK_Test_InstModification_PreviousInstance
  };



static void ind_init(const CMPIContext *ctx) {

  CMPIObjectPath *cop = NULL;
  CMPIStatus      rc;
  
  if (ind_inited==0) {

    printf("--- %s ind_init() called\n",_ClassName);

    cop = CMNewObjectPath( _broker,INDNAMESPACE,_ClassName,&rc);
	 if( CMIsNullObject(cop) ) { 
		printf("Create CMPIObjectPath failed.\n" ); 
		return; 
	}
	
	
    sourceInstance = CMClone(_makeInst_GK_Test(cop , &rc ),&rc);
	
    /* register the broker and context */
    if(ind_reg(_broker, ctx) != IND_OK) {
      printf("--- %s ind_init() failed",_ClassName);
      CMRelease(sourceInstance);
      return; 
    }

    /* register the poll function. check every 10 seconds. */
    if(ind_reg_pollfnc(INDCLASSNAME,
		       "Poll function",
		       check,
		       10,
		       IND_BEHAVIOR_LEVEL_EDGE) != IND_OK) {
      printf("--- %s ind_init() failed: register poll function",_ClassName);
      CMRelease(sourceInstance);
      return;
    }
    
    /* register the dynamic properties and its functions to retrieve data */
    if(ind_set_properties_f(INDNAMESPACE,INDCLASSNAME,
			    GK_TestModification_DYNAMIC_PROPERTIES,
			    GK_TestModification_DYNAMIC_FUNCTIONS,
			    IND_NUMBER_OF_DYNAMIC_PROPERTIES) != IND_OK) {
      printf("--- %s ind_init() failed: register functions of dynamic properties",_ClassName);
      CMRelease(sourceInstance);
      return;
    }
    
    /* tie the properties and polling function together */  
    if(ind_set_classes(INDNAMESPACE,
		       INDCLASSNAME,
		       INDCLASSNAME) != IND_OK) { 
      printf("--- %s ind_init() failed: set connection between poll function and dynamic properties",_ClassName);
      CMRelease(sourceInstance);
      return; 
    }

    ind_inited = 1; 
    printf("--- %s ind_init() exited\n",_ClassName);
  }
}

/* ---------------------------------------------------------------------------*/

/*
Cleanup is called prior to unloading of the provider. This function shall perform any necessary cleanup operation prior to the unloading of the library of which this MI group is part.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
terminating - When true, the terminating argument indicates that the MB is in the process of terminating and that cleanup must be done.
When set to false, the MI may respond with CMPI_RC_DO_NOT_UNLOAD, or CMPI_RC_NEVER_UNLOAD, indicating that unload will interfere with current MI processing. 
Returns:
Function return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred.
CMPI_RC_DO_NOT_UNLOAD Operation successful do not unload now. CMPI_RC_NEVER_UNLOAD Operation successful never unload
*/
CMPIStatus GK_TestIndicationCleanup( 
           CMPIIndicationMI * mi, 
           const CMPIContext * ctx, CMPIBoolean terminate) {
  printf("--- %s CMPI IndicationCleanup() called\n",_ClassName);

  if(sourceInstance)   { CMRelease(sourceInstance); }
  if(previousInstance) { CMRelease(previousInstance); }
  ind_shutdown();
  ind_inited = 0; 
  ind_enabled = 0;

  printf("--- %s CMPI IndicationCleanup() exited\n",_ClassName);
  CMReturn(CMPI_RC_OK);
}

/*
 * Ask the provider to verify whether this filter is allowed.
 * Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
filter - Contains the filter that must be authorized. 
className - Contains the class name extracted from the filter FROM clause. 
op - The name of the class for which monitoring is required. Only the namespace part is set if className is a process indication. 
owner - The owner argument is the destination owner. 
Returns:
This function shall structure containing the service return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. 
CMPI_RC_ERR_FAILED Unspecific error occurred. CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this CMPI_RC_ERR_ACCESS_DENIED Not authorized. 
CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
*/
CMPIStatus GK_TestAuthorizeFilter( 
           CMPIIndicationMI * mi, 
           const CMPIContext * ctx, 
#ifndef CMPI_VER_100
           const CMPIResult * rslt,
#endif
           const CMPISelectExp * filter, 
           const char * indType, 
           const CMPIObjectPath * classPath,
           const char * owner) {
  printf("--- %s CMPI AuthorizeFilter() called\n",_ClassName);
  printf("--- indType : %s,  classPath:%s\n",indType,CMGetCharPtr(classPath));
 

#ifndef CMPI_VER_100
  if (strcasecmp(indType,INDCLASSNAME)==0) {     

	 
    ind_init(ctx);
    printf("--- %s CMPI AuthorizeFilter(): successfully authorized filter",_ClassName);
    CMReturnData(rslt,(CMPIValue*)&(CMPI_true),CMPI_boolean);
  } else {
    printf("--- %s CMPI AuthorizeFilter(): refused to authorize filter",_ClassName);
    CMReturnData(rslt,(CMPIValue*)&(CMPI_false),CMPI_boolean);
  }
  CMReturnDone(rslt);
#endif

  printf("--- %s CMPI AuthorizeFilter() exited\n",_ClassName);


  CMReturn(CMPI_RC_OK);
}

/*
Ask the MI whether polling mode should be used. This function enables very simple MIs to support indications without providing a complete indication support implementation.
When true is returned, the MB will enumerate the instances of this MI at regular intervals and apply indication filters.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
className - The class name extracted from the filter FROM clause. 
filter - The name of the class for which monitoring is required. Only the namespace part is set if eventType is a process indication. 
classPath - The name of the class for which polling would be used. Only the namespace part is set if className is a process indication. 
Returns:
This function shall return a CMPIStatus structure containing the service return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. 
CMPI_RC_ERR_FAILED Unspecific error occurred.CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
*/
CMPIStatus GK_TestMustPoll(
           CMPIIndicationMI * mi, 
           const CMPIContext * ctx, 
#ifndef CMPI_VER_100
           const CMPIResult * rslt,
#endif
           const CMPISelectExp * filter, 
           const char * indType, 
           const CMPIObjectPath * classPath) {
  printf("--- %s CMPI MustPoll() called: NO POLLING\n",_ClassName);  
#ifndef CMPI_VER_100
  CMReturnData(rslt,(CMPIValue*)&(CMPI_false),CMPI_boolean);
  CMReturnDone(rslt);
#endif
  printf("--- %s CMPI MustPoll() exited\n",_ClassName);
  CMReturn(CMPI_RC_OK);
}

/*
Ask the provider to begin monitoring a resource. The function shall begin monitoring the resource according to the filter express only.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
filter - The filter argument contains the filter specification for this subscription to become active. 
className - The class name extracted from the filter FROM clause. 
classPath - The name of the class for which monitoring is required. Only the namespace part is set if eventType is a process indication. 
firstActivation - Set to true if this is the first filter for className. 
Returns:
The function shall return a CMPIStatus structure containing the service return status. The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful.
CMPI_RC_ERR_FAILED Unspecific error occurred.CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. 
CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
*/
CMPIStatus GK_TestActivateFilter(
           CMPIIndicationMI * mi, 
           const CMPIContext * ctx, 
#ifndef CMPI_VER_100
           const CMPIResult * rslt,
#endif
           const CMPISelectExp * filter, 
           const char * indType, 
           const CMPIObjectPath * classPath,
           CMPIBoolean firstActivation) {

  CMPIStatus rc = {CMPI_RC_OK, NULL};
  printf("--- %s CMPI ActivateFilter() called",_ClassName);
  
  ind_init(ctx);
#ifndef CMPI_VER_100
  if (strcasecmp(indType,INDCLASSNAME)==0) {
#endif
    if(ind_set_select(INDNAMESPACE, INDCLASSNAME, filter) == IND_OK) {
      printf("--- %s CMPI ActivateFilter() exited: filter activated (%s)",
		       _ClassName,CMGetCharPtr(CMGetSelExpString(filter,&rc)));
      CMReturn(CMPI_RC_OK);
    }
#ifndef CMPI_VER_100
  }  
#endif
  printf("--- %s CMPI ActivateFilter() exited: filter not activated (%s)",
		   _ClassName,CMGetCharPtr(CMGetSelExpString(filter,&rc)));
  CMReturn(CMPI_RC_ERR_FAILED);
}

/*
Inform the MI that monitoring using this filter should stop. The function invocation mandates the MI to stop monitoring the resource using this filter.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
filter - The filter argument contains the filter specification for this subscription to become active. 
className - The class name extracted from the filter FROM clause. 
classPath - The name of the class for which monitoring is required. Only the namespace part is set if className is a process indication. 
lastActiviation - Set to true if this is the last filter for className. 
Returns:
The function shall return a CMPIStatus structure containing the service return status. The following CMPIrc codes shall be recognized:
CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI. CMPI_RC_ERR_ACCESS_DENIED Not authorized. CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
*/
CMPIStatus GK_TestDeActivateFilter( 
           CMPIIndicationMI * mi, 
           const CMPIContext * ctx, 
#ifndef CMPI_VER_100
           const CMPIResult * rslt,
#endif
           const CMPISelectExp * filter, 
           const char * indType, 
           const CMPIObjectPath *classPath,
           CMPIBoolean lastActivation) {
  printf("--- %s CMPI DeActivateFilter() called\n",_ClassName);

#ifndef CMPI_VER_100
  if (strcasecmp(indType,INDCLASSNAME)==0) {
#endif
    if(ind_unreg_select(INDNAMESPACE, INDCLASSNAME, filter) == IND_OK) {
      printf("--- %s CMPI DeActivateFilter() exited: filter deactivated\n",_ClassName);
      CMReturn(CMPI_RC_OK);
    }
#ifndef CMPI_VER_100
  }  
#endif

  printf("--- %s CMPI DeActivateFilter() exited: filter not deactivated\n",_ClassName);
  CMReturn(CMPI_RC_ERR_FAILED);
}

/*
Tell the MI that indications can now be generated. The MB is now prepared to process indications.
The function is normally called by the MB after having done its intialization and processing of persistent subscription requests.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
Returns:
The function shall return a CMPIStatus structure containing the service return status. 
The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
*/
CMPIStatus GK_TestEnableIndications(CMPIIndicationMI * mi, const CMPIContext * ctx) {
  printf("--- %s CMPI EnableIndications() called\n",_ClassName);

  if(!ind_enabled) {
    /* start the polling mechanism */
    if(ind_start() != IND_OK) { 
      printf("--- %s CMPI EnableIndications() failed: start indication helper\n",_ClassName);
      CMReturn(CMPI_RC_ERR_FAILED); 
    }
    ind_enabled = 1;
  }

  printf("--- %s CMPI EnableIndications() exited\n",_ClassName);
  CMReturn(CMPI_RC_OK);
}
/*
Tell the MI to stop generating indications. MB will not accept any indications until enabled again. 
The function is normally called when the MB is shutting down indication services either temporarily or permanently.
Parameters:
mi - The mi argument is a pointer to a CMPIIndicationMI structure. 
ctx - The ctx argument is a pointer to a CMPIContext structure containing the Invocation Context. 
Returns:
The function shall return a CMPIStatus structure containing the service return status. 
The following CMPIrc codes shall be recognized: CMPI_RC_OK Operation successful. CMPI_RC_ERR_FAILED Unspecific error occurred. 
CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI
*/
CMPIStatus GK_TestDisableIndications(CMPIIndicationMI * mi, const CMPIContext * ctx) {
  printf("--- %s CMPI DisableIndications() called\n",_ClassName);

  if(ind_enabled) {
    /* stop the polling mechanism */
    if(ind_stop() != IND_OK) { 
      printf("--- %s CMPI DisableIndications() failed: stop indication helper\n",_ClassName);
      CMReturn(CMPI_RC_ERR_FAILED); 
    }
    ind_enabled = 0;
  }

  printf("--- %s CMPI DisableIndications() exited\n",_ClassName);
  CMReturn(CMPI_RC_OK);
}




/* ---------------------------------------------------------------------------*/
/*                              Provider Factory                              */
/* ---------------------------------------------------------------------------*/

CMInstanceMIStub( GK_Test, 
                  GK_Test, 
                  _broker, 
                  CMNoHook);

CMMethodMIStub( GK_Test,
                GK_Test, 
                _broker, 
                CMNoHook);

CMIndicationMIStub( GK_Test,
                    GK_Test, 
                    _broker, 
                    CMNoHook);

/* ---------------------------------------------------------------------------*/
/*                    end of GK_Test                      */
/* ---------------------------------------------------------------------------*/

