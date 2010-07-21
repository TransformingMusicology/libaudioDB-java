#include <sys/stat.h>
#include "org_omras2_AudioDB.h"
#include "org_omras2_AudioDB_Mode.h"
#include <jni.h>
#include "audioDB_API.h"
#include <stdlib.h>
#include <fcntl.h>

// Following Ben's lead here!
#define ADB_HEADER_FLAG_L2NORM 0x1
#define ADB_HEADER_FLAG_POWER 0x4
#define ADB_HEADER_FLAG_TIMES 0x20
#define ADB_HEADER_FLAG_REFERENCES 0x40


adb_t* get_handle(JNIEnv *env, jobject obj)
{
	// Fetch the adb pointer

	adb_t *handle;
	
	jclass adbClass = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, adbClass, "adbHandle", "J");
	if(fid == NULL) {
		return;
	}
	
	handle = (adb_t*)((*env)->GetLongField(env, obj, fid)); 
	(*env)->DeleteLocalRef(env, adbClass);
	return handle;	
}

JNIEXPORT jboolean JNICALL Java_org_omras2_AudioDB_audiodb_1create (JNIEnv *env, jobject obj, jstring path, jint datasize, jint ntracks, jint datadim)
{
	char buf[256];
	const char *str;
	str = (*env)->GetStringUTFChars(env, path, NULL);
	if (str == NULL)
		return;

	adb_t *handle;
	handle = audiodb_create(str, datasize, ntracks, datadim);
	if(!handle)
		return JNI_FALSE;
	
	(*env)->ReleaseStringUTFChars(env, path, str);
	return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_org_omras2_AudioDB_audiodb_1open (JNIEnv *env, jobject obj, jstring path, jobject mode)
{
	// TODO: If we have a handle, close the old one.
	if(get_handle(env, obj))
	{
		return;
	}

	jclass modeClass = (*env)->FindClass(env, "org/omras2/AudioDB$Mode");
	jmethodID getNameMethod = (*env)->GetMethodID(env, modeClass, "name", "()Ljava/lang/String;");
	jstring value = (jstring)(*env)->CallObjectMethod(env, mode, getNameMethod);
	const char* openMode = (*env)->GetStringUTFChars(env, value, 0);
	const char* pathVal = (*env)->GetStringUTFChars(env, path, 0);
	int modeVal = 0;
	if(strcmp(openMode, "O_RDWR") == 0)
	{
		modeVal = O_RDWR;
	}
	else if(strcmp(openMode, "O_RDONLY") == 0)
	{
		modeVal = O_RDONLY;
	}
	else
		return;

	adb_t *handle;
	handle = audiodb_open(pathVal, modeVal);
	jclass adbClass = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, adbClass, "adbHandle", "J");
	if(fid == NULL) {
		return;
	}
	(*env)->SetLongField(env, obj, fid, (long)handle);
	(*env)->DeleteLocalRef(env, adbClass);

	return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_org_omras2_AudioDB_audiodb_1close (JNIEnv *env, jobject obj)
{
	adb_t *handle = get_handle(env, obj);
	if(!handle)
		return;

	audiodb_close(handle);
	
	jclass adbClass = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, adbClass, "adbHandle", "J");
	
	if(fid == NULL) {
		return;
	}

	(*env)->SetLongField(env, obj, fid, 0);
	(*env)->DeleteLocalRef(env, adbClass);
}

JNIEXPORT jboolean JNICALL Java_org_omras2_AudioDB_audiodb_1insert_1path(JNIEnv *env, jobject obj, jstring key, jstring features, jstring power, jstring times)
{
	adb_t *handle = get_handle(env, obj);
	if(!handle)
		return JNI_FALSE;
	
	adb_insert_t* ins = (adb_insert_t *)malloc(sizeof(adb_insert_t));
	ins->key = NULL;
	ins->features = NULL;
	ins->power = NULL;
	ins->times = NULL;

	if(key)
		ins->key = (*env)->GetStringUTFChars(env, key, 0);
	if(features)
		ins->features = (*env)->GetStringUTFChars(env, features, 0);
	if(power)
		ins->power = (*env)->GetStringUTFChars(env, power, 0);
	if(times)
		ins->times = (*env)->GetStringUTFChars(env, times, 0);

	int result = audiodb_insert(handle, ins);

	if(result)
		return JNI_FALSE;
	
	return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_org_omras2_AudioDB_query(JNIEnv *env, jobject obj)
{
}

JNIEXPORT jobject JNICALL Java_org_omras2_AudioDB_audiodb_1status(JNIEnv *env, jobject obj)
{
	adb_t *handle = get_handle(env, obj);
	if(!handle)
		return NULL;
	adb_status_t *status;
	status = (adb_status_t *)malloc(sizeof(adb_status_t));
	int flags = audiodb_status(handle, status);	
	
	jclass statusClass = (*env)->FindClass(env, "org/omras2/Status");
	if(statusClass == NULL) {
		return NULL;
	}	
	jmethodID cid = (*env)->GetMethodID(env, statusClass, "<init>", "()V");
	if(cid == NULL) {
		return NULL;
	}

	jobject result = (*env)->NewObject(env, statusClass, cid);

	// This needs a macro!
	jfieldID fid = (*env)->GetFieldID(env, statusClass, "numFiles", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->numFiles);
	
	fid = (*env)->GetFieldID(env, statusClass, "dim", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->dim);
	
	fid = (*env)->GetFieldID(env, statusClass, "dudCount", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->dudCount);
	
	fid = (*env)->GetFieldID(env, statusClass, "nullCount", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->nullCount);
	
	fid = (*env)->GetFieldID(env, statusClass, "length", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->length);
	
	fid = (*env)->GetFieldID(env, statusClass, "dataRegionSize", "I");
	if(fid == NULL) return;
	(*env)->SetIntField(env, result, fid, status->data_region_size);

	// Flags
	
	fid = (*env)->GetFieldID(env, statusClass, "isL2Normed", "Z");
	if(fid == NULL) return;
	(*env)->SetBooleanField(env, result, fid, (status->flags & ADB_HEADER_FLAG_L2NORM));
	
	fid = (*env)->GetFieldID(env, statusClass, "hasPower", "Z");
	if(fid == NULL) return;
	(*env)->SetBooleanField(env, result, fid, (status->flags & ADB_HEADER_FLAG_POWER));
	
	fid = (*env)->GetFieldID(env, statusClass, "hasTimes", "Z");
	if(fid == NULL) return;
	(*env)->SetBooleanField(env, result, fid, (status->flags & ADB_HEADER_FLAG_TIMES));
	
	fid = (*env)->GetFieldID(env, statusClass, "hasReferences", "Z");
	if(fid == NULL) return;
	(*env)->SetBooleanField(env, result, fid, (status->flags & ADB_HEADER_FLAG_REFERENCES));

	(*env)->DeleteLocalRef(env, statusClass);

	return result;
}

