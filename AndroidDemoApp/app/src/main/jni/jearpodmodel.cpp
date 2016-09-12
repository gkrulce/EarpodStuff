#include <jni.h>
#include "EarpodModel.hpp"
extern "C" {
JNIEXPORT jintArray JNICALL
Java_com_gkrulce_androiddemoapp_JEarpodModel_readJni(JNIEnv *env, jobject instance,
                                                     jshortArray samples_) {
    jshort *samples = env->GetShortArrayElements(samples_, 0);
    int arrSize = env->GetArrayLength(samples_);
    std::vector<unsigned char> toSend(arrSize * 2);
    for (int i = 0; i < arrSize; ++i) {
        unsigned char lsb = samples[i];
        unsigned char msb = (samples[i] >> 8);
        toSend.at(2 * i) = lsb;
        toSend.at(2 * i + 1) = msb;
    }
    EarpodModel model;
    std::vector<EarpodModel::Token> emBeg = model.read(toSend);
    std::vector<EarpodModel::Token> emEnd = model.readEOF();
    std::vector<EarpodModel::Token> emRet;
    emRet.insert(emRet.end(), emBeg.begin(), emBeg.end());
    emRet.insert(emRet.end(), emEnd.begin(), emEnd.end());

    std::vector<int> intRet(emRet.size());
    // These mappings match those in java/JEarpodModel.java
    for (int i = 0; i < emRet.size(); ++i) {
        switch (emRet.at(i)) {
            case EarpodModel::VOLUME_UP:
                intRet.at(i) = 0;
                break;
            case EarpodModel::VOLUME_DOWN:
                intRet.at(i) = 1;
                break;
            case EarpodModel::NOISE:
                intRet.at(i) = 2;
                break;
        }
    }
    jintArray jRet = env->NewIntArray(emRet.size());
    env->SetIntArrayRegion(jRet, 0, emRet.size(), &intRet[0]);
    return jRet;
}
}