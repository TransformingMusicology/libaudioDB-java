/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class AudioDB */

#ifndef _Included_AudioDB
#define _Included_AudioDB
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     AudioDB
 * Method:    audiodb_create
 * Signature: (Ljava/lang/String;III)Z
 */
JNIEXPORT jboolean JNICALL Java_AudioDB_audiodb_1create
  (JNIEnv *, jobject, jstring, jint, jint, jint);

/*
 * Class:     AudioDB
 * Method:    audiodb_open
 * Signature: (Ljava/lang/String;LAudioDB/Mode;)Z
 */
JNIEXPORT jboolean JNICALL Java_AudioDB_audiodb_1open
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     AudioDB
 * Method:    insert
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_AudioDB_insert
  (JNIEnv *, jobject);

/*
 * Class:     AudioDB
 * Method:    query
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_AudioDB_query
  (JNIEnv *, jobject);

/*
 * Class:     AudioDB
 * Method:    status
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_AudioDB_status
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif