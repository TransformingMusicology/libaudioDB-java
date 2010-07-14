#include "org_omras2_AudioDB.h"
#include "org_omras2_AudioDB_Mode.h"
#include <jni.h>
#include "audioDB_API.h"
#include <stdlib.h>

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
	
	jclass adbClass = (*env)->GetObjectClass(env, obj);
	jfieldID fid = (*env)->GetFieldID(env, adbClass, "adbHandle", "J");
	if(fid == NULL) {
		return;
	}
	(*env)->SetLongField(env, obj, fid, (long)handle);
	(*env)->DeleteLocalRef(env, adbClass);
	(*env)->ReleaseStringUTFChars(env, path, str);
	return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_org_omras2_AudioDB_audiodb_1open (JNIEnv *env, jobject obj, jstring path, jobject mode)
{
	jclass modeClass = (*env)->FindClass(env, "org/omras2/AudioDB$Mode");
	jmethodID getNameMethod = (*env)->GetMethodID(env, modeClass, "name", "()Ljava/lang/String;");
	jstring value = (jstring)(*env)->CallObjectMethod(env, mode, getNameMethod);
	const char* openMode = (*env)->GetStringUTFChars(env, value, 0);

	return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_org_omras2_AudioDB_insert(JNIEnv *env, jobject obj)
{
}

JNIEXPORT void JNICALL Java_org_omras2_AudioDB_query(JNIEnv *env, jobject obj)
{
}

JNIEXPORT jobject JNICALL Java_org_omras2_AudioDB_audiodb_1status(JNIEnv *env, jobject obj)
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

	fid = (*env)->GetFieldID(env, statusClass, "numFiles", "I");
	if(fid == NULL) {
		return;
	}
	(*env)->SetIntField(env, result, fid, status->numFiles);

	(*env)->DeleteLocalRef(env, statusClass);

	return result;
}

