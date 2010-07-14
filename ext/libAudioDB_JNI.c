#include "org_omras2_AudioDB.h"
#include "org_omras2_AudioDB_Mode.h"
#include <jni.h>
#include "audioDB_API.h"


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

JNIEXPORT void JNICALL Java_org_omras2_AudioDB_audiodb_1status(JNIEnv *env, jobject obj)
{
}

