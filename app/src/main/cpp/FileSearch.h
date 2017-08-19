//
// Created by owl31 on 2017/8/15.
//

#include <jni.h>

extern "C"{
jobject
Java_filesearch_FileSearch_searchFileBySuffix(JNIEnv *env, jclass type, jstring rootPath_,
                                                   jstring suffix_);

void scan_dir2(JNIEnv *env,const char *directory,const char *suffix, jobject list, jmethodID add_list);
}