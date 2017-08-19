#include <jni.h>
#include <string>
#include "FileSearch.h"
#include<dirent.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

extern "C"
JNIEXPORT jobject JNICALL
Java_filesearch_FileSearch_searchFileBySuffix(JNIEnv *env, jclass type, jstring rootPath_,
                                                   jstring suffix_) {
    const char *rootPath = env->GetStringUTFChars(rootPath_, 0);
    const char *suffix = env->GetStringUTFChars(suffix_, 0);

    __android_log_print(ANDROID_LOG_ERROR, "FileSearch","path:%s---rootPath:%s-----suffix:%s"," ",rootPath,suffix);
    jclass class_list = env->FindClass("java/util/ArrayList");
    jmethodID constructor_list = env->GetMethodID(class_list,"<init>","()V");
    jmethodID add_list = env->GetMethodID(class_list,"add","(Ljava/lang/Object;)Z");
    jobject list = env->NewObject(class_list,constructor_list);
    scan_dir2(env,rootPath,suffix,list,add_list);

    env->ReleaseStringUTFChars(rootPath_, rootPath);
    env->ReleaseStringUTFChars(suffix_, suffix);
    return list;
}


extern "C"
void scan_dir2(JNIEnv *env,const char *directory,const char *suffix, jobject list, jmethodID add_list) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(directory)) == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, "FileSearch","dir open fail !!! path:%s",directory);
        return;
    }
    chdir(directory);
    jstring jstr;

    while ((entry = readdir(dp)) != NULL) {
        stat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (entry->d_name[0] != '.')) {
                scan_dir2(env, entry->d_name, suffix, list, add_list);
            }
        } else {
            size_t size = strlen(entry->d_name);
            if (entry->d_name[0] != '.'  //隐藏文件
                && strcmp( ( entry->d_name + (size - 4) ) , suffix) == 0) {
                char* parentPath = (char*)malloc(1024);
                char* absolutePath = (char*)malloc(1024);
                getcwd(parentPath,1024);
                strcpy(absolutePath,parentPath);
                const char *p = "/";
                absolutePath = strcat(absolutePath,p);
                absolutePath = strcat(absolutePath,entry->d_name);
                jstr = env->NewStringUTF(absolutePath);
                env->CallBooleanMethod(list,add_list,jstr);
                __android_log_print(ANDROID_LOG_ERROR, "FileSearch","find apk:%s",absolutePath);
                free(parentPath);
                parentPath = NULL;
                free(absolutePath);
                absolutePath = NULL;
            }
        }
    }
    chdir("..");
    closedir(dp);
}