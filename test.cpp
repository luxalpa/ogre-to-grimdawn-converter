//struct someObject {
//    void *anything;
//    int *pData;
//};

//char sub_11A3FD0(someObject *a1) {
//    unsigned int v3; // eax@1 MAPDST
//    char v8; // cl@5
//    int relativeAddrToBone; // esi@12
//    size_t *v16; // eax@13
//    size_t v17; // edi@13
//    _BYTE *v18; // ecx@13
//    const char *v19; // eax@15
//    int *v20; // eax@19
//    int v21; // ecx@19
//    int *v22; // eax@19
//    int v23; // ecx@19
//    int *v24; // eax@19
//    char *v25; // ecx@19
//    int v26; // xmm0_4@19
//    int *v27; // eax@19
//    int v28; // xmm0_4@19
//    int *v29; // eax@19
//    int v30; // xmm0_4@19
//    int *v31; // eax@19
//    int v32; // xmm0_4@19
//    int *v33; // eax@19
//    int v34; // xmm0_4@19
//    int *v35; // eax@19
//    int v36; // xmm0_4@19
//    int *v37; // eax@19
//    int v38; // xmm0_4@19
//    int *v39; // eax@19
//    int v40; // xmm0_4@19
//    int *v41; // eax@19
//    int v42; // xmm0_4@19
//    int *v43; // eax@19
//    int v44; // xmm0_4@19
//    int *v45; // eax@19
//    int v46; // xmm0_4@19
//    int *v47; // eax@19
//    int v48; // xmm0_4@19
//    char *v49; // ecx@20
//    unsigned int v50; // eax@22
//    char v51; // cl@28
//    unsigned int v52; // edi@28
//    signed int v53; // esi@30
//    int v54; // eax@31
//    signed int *v55; // ecx@34
//    _DWORD *v56; // edx@34
//    char *v57; // edi@36
//    signed int v58; // eax@36
//    int v59; // ecx@38
//    unsigned int v60; // esi@39
//    unsigned int v61; // edi@39
//    unsigned int v62; // edx@39
//    int v63; // ecx@49
//    unsigned int v64; // esi@50
//    unsigned int v65; // edi@50
//    unsigned int v66; // edx@50
//    unsigned int *v67; // eax@62
//    unsigned int v68; // ecx@62
//    unsigned int v69; // edx@62
//    someObject *v70; // ecx@63
//    int *v71; // eax@63
//    int v72; // xmm0_4@63
//    int v73; // xmm0_4@63
//    int v74; // xmm0_4@63
//    int v75; // xmm0_4@63
//    int v76; // xmm0_4@63
//    int v77; // xmm0_4@63
//    int v78; // eax@65
//    unsigned int v79; // edi@68
//    unsigned int v80; // esi@68
//    unsigned int v81; // edx@68
//    int v82; // eax@74
//    int v83; // eax@77
//    unsigned int v84; // edi@83
//    unsigned int v85; // esi@83
//    unsigned int v86; // edx@83
//    unsigned int v87; // eax@98
//    unsigned int v88; // eax@103
//    unsigned int v89; // ebx@105
//    int v90; // eax@112
//    __int128 v91; // [sp+4h] [bp-5Ch]@63
//    __int64 v92; // [sp+14h] [bp-4Ch]@63
//    char *Source; // [sp+1Ch] [bp-44h]@13
//    void *Memory; // [sp+28h] [bp-38h]@62
//    unsigned int v95; // [sp+2Ch] [bp-34h]@13
//    unsigned int v96; // [sp+30h] [bp-30h]@13
//    someObject *v97; // [sp+34h] [bp-2Ch]@1
//    unsigned int v98; // [sp+38h] [bp-28h]@5
//    unsigned int numNodes; // [sp+3Ch] [bp-24h]@5 MAPDST
//    int v100; // [sp+40h] [bp-20h]@7
//    unsigned int numMaterials; // [sp+44h] [bp-1Ch]@5 MAPDST
//    unsigned int numBones; // [sp+4Ch] [bp-14h]@5 MAPDST
//    char v104; // [sp+53h] [bp-Dh]@5
//    int v105; // [sp+5Ch] [bp-4h]@13

//    v97 = a1;

//    if ( (_WORD)*a1->pData != 'DM' || BYTE2(*a1->pData) != 'L' ) {
//        sub_11A2760(_acrt_iob_func(2), "Incompatible file type\n");
//        return 0;
//    }
//    if ( BYTE3(*a1->pData) != 7 ) {
//        sub_11A2760(_acrt_iob_func(2), "Incompatible file version\n");
//        return 0;
//    }

//    a1->pData++;

//    numMaterials = *a1->pData;

//    a1->pData++;
//    numNodes = *a1->pData;
//    a1->pData++;
//    numBones = *a1->pData;
//    a1->pData++;
//    v98 = *a1->pData;
//    a1->pData++;

//    v104 = 1;
//    v8 = 1;
//    for(int i = 0; i < numMaterials; i++) {
//        if ( !v8 )
//            break;
//        v100 = sub_11A3DF0(a1);
//        if ( v100 )
//        {
//            sub_11A7340(&v100);
//            v8 = v104;
//        }
//        else
//        {
//            sub_11A2760(_acrt_iob_func(2), "Error parsing material\n");
//            v8 = 0;
//            v104 = 0;
//        }
//    }
//    sub_11A7900(numBones);
//    v3 = 0;

//    relativeAddrToBone = 0;
//    for(int i = 0; i < numBones; i++) {
//        v96 = 15;
//        v95 = 0;
//        LOBYTE(Source) = 0;
//        v105 = 0;
//        v16 = (size_t *)a1->pData;
//        v17 = *(size_t *)a1->pData;
//        a1->pData++;
//        v18 = a1->pData;
//        if ( v17 > 0x100000
//             || (void *)(v17 + v18 - a1->vtable) > a1[1].vtable
//             || (sub_11A7B20(v18, v17), a1->pData = (int *)((char *)a1->pData + v17), v19 = Source, v96 < 0x10) )
//        {
//            v19 = (const char *)&Source;
//        }
//        strncpy((char *)memBones + relativeAddrToBone, v19, 0x20u);
//        *((_BYTE *)memBones + relativeAddrToBone + 31) = 0;
//        if ( v95 >= 0x20 )
//            sub_11A2790("Truncating bone name '%s' to '%s'\n");
//        v20 = a1->pData;
//        v21 = *v20;
//        a1->pData = v20 + 1;
//        *(_DWORD *)((char *)memBones + relativeAddrToBone + 36) = v21;
//        v22 = a1->pData;
//        v23 = *v22;
//        a1->pData = v22 + 1;
//        *(_DWORD *)((char *)memBones + relativeAddrToBone + 32) = v23;
//        v24 = a1->pData;
//        v25 = (char *)memBones;
//        v26 = *v24;
//        a1->pData = v24 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 40] = v26;
//        v27 = a1->pData;
//        v28 = *v27;
//        a1->pData = v27 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 44] = v28;
//        v29 = a1->pData;
//        v30 = *v29;
//        a1->pData = v29 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 48] = v30;
//        v31 = a1->pData;
//        v32 = *v31;
//        a1->pData = v31 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 52] = v32;
//        v33 = a1->pData;
//        v34 = *v33;
//        a1->pData = v33 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 56] = v34;
//        v35 = a1->pData;
//        v36 = *v35;
//        a1->pData = v35 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 60] = v36;
//        v37 = a1->pData;
//        v38 = *v37;
//        a1->pData = v37 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 64] = v38;
//        v39 = a1->pData;
//        v40 = *v39;
//        a1->pData = v39 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 68] = v40;
//        v41 = a1->pData;
//        v42 = *v41;
//        a1->pData = v41 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 72] = v42;
//        v43 = a1->pData;
//        v44 = *v43;
//        a1->pData = v43 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 76] = v44;
//        v45 = a1->pData;
//        v46 = *v45;
//        a1->pData = v45 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 80] = v46;
//        v47 = a1->pData;
//        v48 = *v47;
//        a1->pData = v47 + 1;
//        *(_DWORD *)&v25[relativeAddrToBone + 84] = v48;
//        v105 = -1;
//        if ( v96 >= 0x10 )
//        {
//            v49 = Source;
//            if ( v96 + 1 >= 0x1000 )
//            {
//                if ( (unsigned __int8)Source & 0x1F )
//                    goto LABEL_99;
//                v50 = *((_DWORD *)Source - 1);
//                if ( v50 >= (unsigned int)Source )
//                    goto LABEL_99;
//                v49 = &Source[-v50];
//                if ( (unsigned int)&Source[-v50] < 4 || (unsigned int)v49 > 0x23 )
//                    goto LABEL_99;
//                v49 = (char *)*((_DWORD *)Source - 1);
//            }
//            j_free(v49);
//        }
//        relativeAddrToBone += 88;
//    }
//    v51 = v104;
//    v52 = 0;
//    v3 = 0;
//    v100 = 0;
//    if ( !numNodes )
//    {
//LABEL_60:
//        v100 = 0;
//        if ( v98 )
//            goto LABEL_61;
//        return v104;
//    }
//    while ( 1 )
//    {
//        if ( !v51 )
//            goto LABEL_60;
//        v53 = sub_11A3CC0(a1);
//        numBones = v53;
//        if ( v53 )
//            break;
//        v54 = _acrt_iob_func(2);
//        sub_11A2760(v54, "Error parsing node\n");
//        v51 = 0;
//        v104 = 0;
//LABEL_59:
//        if ( ++v100 >= numNodes )
//            goto LABEL_60;
//    }
//    if ( (**(int (__thiscall ***)(_DWORD))v53)(v53) == 1 )
//    {
//        v3 = GAME::SceneMeshNode::GetNumVertices((GAME::SceneMeshNode *)v53) + v52;
//        if ( v3 > 0x10000 )
//        {
//            v83 = _acrt_iob_func(2);
//            sub_11A2760(v83, "Model has more than the maximum of %u vertices.\n");
//            return 0;
//        }
//    }
//    v55 = (signed int *)dword_11B556C;
//    v56 = dword_11B5568;
//    if ( (unsigned int)&numBones >= dword_11B556C || dword_11B5568 > &numBones )
//    {
//        v57 = (char *)dword_11B5570;
//        if ( dword_11B556C == dword_11B5570 && (unsigned int)((dword_11B5570 - dword_11B556C) >> 2) < 1 )
//        {
//            v63 = (dword_11B556C - (signed int)dword_11B5568) >> 2;
//            if ( (unsigned int)(0x3FFFFFFF - v63) < 1 )
//                goto LABEL_79;
//            v64 = v63 + 1;
//            v65 = (dword_11B5570 - (signed int)dword_11B5568) >> 2;
//            v66 = 0;
//            if ( 0x3FFFFFFF - (v65 >> 1) >= v65 )
//                v66 = v65 + (v65 >> 1);
//            if ( v66 >= v64 )
//                v64 = v66;
//            sub_11A9E10(v64);
//            v55 = (signed int *)dword_11B556C;
//            v53 = numBones;
//        }
//        if ( !v55 )
//            goto LABEL_58;
//        *v55 = v53;
//        goto LABEL_57;
//    }
//    v57 = (char *)dword_11B5570;
//    v58 = ((char *)&numBones - (_BYTE *)dword_11B5568) >> 2;
//    numMaterials = ((char *)&numBones - (_BYTE *)dword_11B5568) >> 2;
//    if ( dword_11B556C != dword_11B5570 )
//    {
//LABEL_45:
//        if ( !v55 )
//        {
//LABEL_58:
//            v52 = v3;
//            dword_11B556C = (int)(v55 + 1);
//            v51 = v104;
//            goto LABEL_59;
//        }
//        *v55 = v56[v58];
//LABEL_57:
//        v55 = (signed int *)dword_11B556C;
//        goto LABEL_58;
//    }
//    if ( (unsigned int)((dword_11B5570 - dword_11B556C) >> 2) >= 1 )
//    {
//LABEL_44:
//        v58 = numMaterials;
//        goto LABEL_45;
//    }
//    v59 = (dword_11B556C - (signed int)dword_11B5568) >> 2;
//    if ( (unsigned int)(0x3FFFFFFF - v59) >= 1 )
//    {
//        v60 = v59 + 1;
//        v61 = (dword_11B5570 - (signed int)dword_11B5568) >> 2;
//        v62 = 0;
//        if ( 0x3FFFFFFF - (v61 >> 1) >= v61 )
//            v62 = v61 + (v61 >> 1);
//        if ( v62 >= v60 )
//            v60 = v62;
//        sub_11A9E10(v60);
//        v55 = (signed int *)dword_11B556C;
//        v56 = dword_11B5568;
//        goto LABEL_44;
//    }
//LABEL_79:
//    while ( 1 )
//    {
//        std::_Xlength_error("vector<T> too long");
//LABEL_80:
//        if ( (someObject *)v53 != a1 )
//            break;
//        if ( (unsigned int)(((signed int)a1 - v53) / 24) >= 1 )
//            goto LABEL_88;
//        v53 -= (signed int)v57;
//        v57 = (char *)(v53 / 24);
//        if ( (unsigned int)(178956970 - v53 / 24) >= 1 )
//        {
//            v84 = (unsigned int)(v57 + 1);
//            v85 = ((char *)a1 - (_BYTE *)Memory) / 24;
//            v86 = 0;
//            if ( 178956970 - (v85 >> 1) >= v85 )
//                v86 = v85 + (v85 >> 1);
//            if ( v86 >= v84 )
//                v84 = v86;
//            sub_11A9F30(v84);
//            a1 = (someObject *)v96;
//            v53 = v95;
//            v57 = (char *)Memory;
//LABEL_88:
//            v69 = numBones;
//            break;
//        }
//    }
//    if ( v53 )
//    {
//        *(_OWORD *)v53 = v91;
//        _mm_storel_epi64((__m128i *)(v53 + 16), _mm_loadl_epi64((const __m128i *)&v92));
//    }
//    while ( 1 )
//    {
//        ++v69;
//        v53 += 24;
//        v95 = v53;
//        numBones = v69;
//        if ( v69 >= numMaterials )
//            break;
//LABEL_63:
//        v70 = v97;
//        v71 = v97->pData;
//        v72 = *v71;
//        ++v71;
//        v97->pData = v71;
//        LODWORD(v91) = v72;
//        v73 = *v71;
//        ++v71;
//        v70->pData = v71;
//        DWORD1(v91) = v73;
//        v74 = *v71;
//        ++v71;
//        v70->pData = v71;
//        DWORD2(v91) = v74;
//        v75 = *v71;
//        ++v71;
//        v70->pData = v71;
//        DWORD3(v91) = v75;
//        v76 = *v71;
//        ++v71;
//        v70->pData = v71;
//        LODWORD(v92) = v76;
//        v77 = *v71;
//        v70->pData = v71 + 1;
//        HIDWORD(v92) = v77;
//        if ( (unsigned int)&v91 >= v53 || v57 > (char *)&v91 )
//            goto LABEL_80;
//        v78 = ((char *)&v91 - v57) / 24;
//        numNodes = ((char *)&v91 - v57) / 24;
//        if ( (someObject *)v53 == a1 )
//        {
//            if ( (unsigned int)(((signed int)a1 - v53) / 24) < 1 )
//            {
//                v53 -= (signed int)v57;
//                v57 = (char *)(v53 / 24);
//                if ( (unsigned int)(178956970 - v53 / 24) < 1 )
//                    goto LABEL_79;
//                v79 = (unsigned int)(v57 + 1);
//                v80 = ((char *)a1 - (_BYTE *)Memory) / 24;
//                v81 = 0;
//                if ( 178956970 - (v80 >> 1) >= v80 )
//                    v81 = v80 + (v80 >> 1);
//                if ( v81 >= v79 )
//                    v79 = v81;
//                sub_11A9F30(v79);
//                a1 = (someObject *)v96;
//                v53 = v95;
//                v57 = (char *)Memory;
//            }
//            v78 = numNodes;
//        }
//        v82 = 3 * v78;
//        if ( v53 )
//        {
//            *(_OWORD *)v53 = *(_OWORD *)&v57[8 * v82];
//            _mm_storel_epi64((__m128i *)(v53 + 16), _mm_loadl_epi64((const __m128i *)&v57[8 * v82 + 16]));
//        }
//        v69 = numBones;
//    }
//    while ( (unsigned __int8)sub_11A3E80(&Memory) )
//    {
//        sub_11A7840(&Memory);
//        v105 = -1;
//        if ( Memory )
//        {
//            sub_11A9ED0(Memory, (signed int)(v96 - (_DWORD)Memory) / 24);
//            Memory = 0;
//            v95 = 0;
//            v96 = 0;
//        }
//        if ( ++v100 >= v98 )
//            return v104;
//        a1 = v97;
//        v51 = v104;
//LABEL_61:
//        if ( !v51 )
//            return v104;
//        v67 = (unsigned int *)a1->pData;
//        v57 = 0;
//        v53 = 0;
//        Memory = 0;
//        v95 = 0;
//        v68 = *v67;
//        a1->pData = (int *)(v67 + 1);
//        a1 = 0;
//        numMaterials = v68;
//        v96 = 0;
//        v69 = 0;
//        v105 = 1;
//        numBones = 0;
//        if ( v68 )
//            goto LABEL_63;
//    }
//    a1 = (someObject *)Memory;
//    if ( !Memory )
//        return 0;
//    v49 = (char *)(v96 - (_DWORD)Memory);
//    v87 = (signed int)(v96 - (_DWORD)Memory) / 24;
//    if ( v87 > 0xAAAAAAA )
//LABEL_99:
//        v87 = invalid_parameter_noinfo_noreturn(v49);
//    if ( 24 * v87 >= 0x1000 )
//    {
//        if ( (unsigned __int8)a1 & 0x1F )
//            invalid_parameter_noinfo_noreturn(v49);
//        v88 = (unsigned int)a1[-1].pData;
//        if ( v88 >= (unsigned int)a1 )
//            v88 = invalid_parameter_noinfo_noreturn(v49);
//        v89 = (unsigned int)a1 - v88;
//        if ( v89 < 4 )
//            v88 = invalid_parameter_noinfo_noreturn(v49);
//        if ( v89 > 0x23 )
//            v88 = invalid_parameter_noinfo_noreturn(v49);
//        a1 = (someObject *)v88;
//    }
//    j_free(a1);
//    return 0;
//}
