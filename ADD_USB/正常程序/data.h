
/*============================================ 头文件说明 ============================================================
头文件名称：data.h
头文件用途：用于数据线性化的定义
说      明：该文件是供开发人员修改
================================================ END ===============================================================*/
/*============================
格式： data_输入电压值MV_zy_增益值     AD7711读到的"实际"电压值，这个实际电压值，就是使用文件"进行电压线性化的辅助代码.HEX"读取的屏显值。
例如：使用信号发生器发出  0.400mv 电压，在增益3的状态下，屏上读取的电压是20.741mV，那么我们这个数据就可以记录如下：
#define   data_0400_zy_3    20741
=============================*/
//  无输入
#define   data_0000_zy_0    4340     
#define   data_0000_zy_1    7430
#define   data_0000_zy_2    14870
#define   data_0000_zy_3    27000
#define   data_0000_zy_4    60800
#define   data_0000_zy_5    70000
#define   data_0000_zy_6    71000
#define   data_0000_zy_7    16000
#define   data_0000_zy_8    7000
#define   data_0000_zy_9    6000

///////////////////////////////////////////////////////////////////////////////////////////
///////电压部分，于2010-09-07补充
#define   data_00001_zy_0    386 //  00001uV
#define   data_00001_zy_1    730
#define   data_00001_zy_2    740
#define   data_00001_zy_3    750
#define   data_00001_zy_4    760
#define   data_00001_zy_5    770
#define   data_00001_zy_6    780
#define   data_00001_zy_7    790
#define   data_00001_zy_8    3000
#define   data_00001_zy_9    19000

/*
#define   data_00002_zy_0    2602 //  00002uV
#define   data_00002_zy_1    2669
#define   data_00002_zy_2    2868
#define   data_00002_zy_3    3180
#define   data_00002_zy_4    3218
#define   data_00002_zy_5    3327
#define   data_00002_zy_6    3391
#define   data_00002_zy_7    3485
#define   data_00002_zy_8    4335
#define   data_00002_zy_9    6476

#define   data_00003_zy_0    2604 //  00003uV
#define   data_00003_zy_1    2705
#define   data_00003_zy_2    2871
#define   data_00003_zy_3    3314
#define   data_00003_zy_4    3251
#define   data_00003_zy_5    3383
#define   data_00003_zy_6    3433
#define   data_00003_zy_7    3804
#define   data_00003_zy_8    5191
#define   data_00003_zy_9    8848

#define   data_00004_zy_0    2606 //  00004uV
#define   data_00004_zy_1    2715
#define   data_00004_zy_2    2875
#define   data_00004_zy_3    3317
#define   data_00004_zy_4    3375
#define   data_00004_zy_5    3508
#define   data_00004_zy_6    4797
#define   data_00004_zy_7    4857
#define   data_00004_zy_8    6877
#define   data_00004_zy_9    11385
*/

#define   data_00005_zy_0    390 //  00005uV
#define   data_00005_zy_1    830
#define   data_00005_zy_2    1660
#define   data_00005_zy_3    1895
#define   data_00005_zy_4    1901
#define   data_00005_zy_5    1911
#define   data_00005_zy_6    1930
#define   data_00005_zy_7    2231
#define   data_00005_zy_8    7100
#define   data_00005_zy_9    21710

/*
#define   data_00006_zy_0    2610 //  00006uV
#define   data_00006_zy_1    2719
#define   data_00006_zy_2    2880
#define   data_00006_zy_3    3322
#define   data_00006_zy_4    3381
#define   data_00006_zy_5    3614
#define   data_00006_zy_6    4880
#define   data_00006_zy_7    5501
#define   data_00006_zy_8    8817
#define   data_00006_zy_9    15951

#define   data_00007_zy_0    2612 //  00007uV
#define   data_00007_zy_1    2721
#define   data_00007_zy_2    2885
#define   data_00007_zy_3    3325
#define   data_00007_zy_4    3385
#define   data_00007_zy_5    3617
#define   data_00007_zy_6    4885
#define   data_00007_zy_7    5861
#define   data_00007_zy_8    9880
#define   data_00007_zy_9    18354

#define   data_00008_zy_0    2614 //  00008uV
#define   data_00008_zy_1    2723
#define   data_00008_zy_2    2887
#define   data_00008_zy_3    3328
#define   data_00008_zy_4    3388
#define   data_00008_zy_5    3620
#define   data_00008_zy_6    4888
#define   data_00008_zy_7    6500
#define   data_00008_zy_8    11354
#define   data_00008_zy_9    20676

#define   data_00009_zy_0    2616 //  00009uV
#define   data_00009_zy_1    2725
#define   data_00009_zy_2    2889
#define   data_00009_zy_3    3330
#define   data_00009_zy_4    3390
#define   data_00009_zy_5    3623
#define   data_00009_zy_6    4900
#define   data_00009_zy_7    7181
#define   data_00009_zy_8    12669
#define   data_00009_zy_9    23248
*/

#define   data_00010_zy_0    400 //  00010uV
#define   data_00010_zy_1    833
#define   data_00010_zy_2    1666
#define   data_00010_zy_3    3332
#define   data_00010_zy_4    3392
#define   data_00010_zy_5    3626
#define   data_00010_zy_6    3404
#define   data_00010_zy_7    3841
#define   data_00010_zy_8    10986
#define   data_00010_zy_9    24420

/*
#define   data_00011_zy_0    2625 //  00011uV
#define   data_00011_zy_1    2734
#define   data_00011_zy_2    2900
#define   data_00011_zy_3    3342
#define   data_00011_zy_4    3403
#define   data_00011_zy_5    3759
#define   data_00011_zy_6    5188
#define   data_00011_zy_7    8222
#define   data_00011_zy_8    14867
#define   data_00011_zy_9    28138

#define   data_00012_zy_0    2640 //  00012uV
#define   data_00012_zy_1    2742
#define   data_00012_zy_2    2933
#define   data_00012_zy_3    3350
#define   data_00012_zy_4    3420
#define   data_00012_zy_5    3835
#define   data_00012_zy_6    5294
#define   data_00012_zy_7    8677
#define   data_00012_zy_8    15748
#define   data_00012_zy_9    30492
*/

#define   data_00015_zy_0    410 //  00015uV
#define   data_00015_zy_1    839
#define   data_00015_zy_2    1678
#define   data_00015_zy_3    3357
#define   data_00015_zy_4    3501
#define   data_00015_zy_5    4046
#define   data_00015_zy_6    4060
#define   data_00015_zy_7    5553
#define   data_00015_zy_8    14215
#define   data_00015_zy_9    26852

#define   data_00020_zy_0    420 //  00020uV
#define   data_00020_zy_1    840
#define   data_00020_zy_2    1681
#define   data_00020_zy_3    3362
#define   data_00020_zy_4    4160
#define   data_00020_zy_5    4751
#define   data_00020_zy_6    5420
#define   data_00020_zy_7    8750
#define   data_00020_zy_8    18376
#define   data_00020_zy_9    35003
/*
#define   data_00030_zy_0    2659 //  00030uV
#define   data_00030_zy_1    2777
#define   data_00030_zy_2    2986
#define   data_00030_zy_3    3452
#define   data_00030_zy_4    4159
#define   data_00030_zy_5    6190
#define   data_00030_zy_6    10408
#define   data_00030_zy_7    19666
#define   data_00030_zy_8    37672
#define   data_00030_zy_9    74362
*/
#define   data_00040_zy_0    456 //  00040uV
#define   data_00040_zy_1    912
#define   data_00040_zy_2    1824
#define   data_00040_zy_3    3649
#define   data_00040_zy_4    4812
#define   data_00040_zy_5    7584
#define   data_00040_zy_6    13530
#define   data_00040_zy_7    18152
#define   data_00040_zy_8    35003
#define   data_00040_zy_9    70006
/*
#define   data_00050_zy_0    2663 //  00050uV
#define   data_00050_zy_1    2783
#define   data_00050_zy_2    3079
#define   data_00050_zy_3    3928
#define   data_00050_zy_4    5528
#define   data_00050_zy_5    9200
#define   data_00050_zy_6    16726
#define   data_00050_zy_7    32348
#define   data_00050_zy_8    62628
#define   data_00050_zy_9    124079
*/
#define   data_00060_zy_0    523 //  00060 uV
#define   data_00060_zy_1    1047
#define   data_00060_zy_2    2094
#define   data_00060_zy_3    4187
#define   data_00060_zy_4    6302
#define   data_00060_zy_5    10696
#define   data_00060_zy_6    19856
#define   data_00060_zy_7    28616
#define   data_00060_zy_8    56294
#define   data_00060_zy_9    112589
/*
#define   data_00070_zy_0    2677 //  00070 uV
#define   data_00070_zy_1    2791
#define   data_00070_zy_2    3281
#define   data_00070_zy_3    4485
#define   data_00070_zy_4    6954
#define   data_00070_zy_5    12152
#define   data_00070_zy_6    22778
#define   data_00070_zy_7    44139
#define   data_00070_zy_8    87429
#define   data_00070_zy_9    173556
*/
#define   data_00080_zy_0    594 //  00080 uV
#define   data_00080_zy_1    1187
#define   data_00080_zy_2    2373
#define   data_00080_zy_3    4747
#define   data_00080_zy_4    7440
#define   data_00080_zy_5    13385
#define   data_00080_zy_6    24919
#define   data_00080_zy_7    38032
#define   data_00080_zy_8    75128
#define   data_00080_zy_9    150231
/*
#define   data_00090_zy_0    2685 //  00090 uV
#define   data_00090_zy_1    2918
#define   data_00090_zy_2    3587
#define   data_00090_zy_3    5135
#define   data_00090_zy_4    8296
#define   data_00090_zy_5    14969
#define   data_00090_zy_6    28221
#define   data_00090_zy_7    55100
#define   data_00090_zy_8    108295
#define   data_00090_zy_9    215678
*/
#define   data_00100_zy_0    675 //  00100 uV
#define   data_00100_zy_1    1350
#define   data_00100_zy_2    2700
#define   data_00100_zy_3    5400
#define   data_00100_zy_4    8924
#define   data_00100_zy_5    16355
#define   data_00100_zy_6    31434
#define   data_00100_zy_7    47894
#define   data_00100_zy_8    94831
#define   data_00100_zy_9    189662
/*
#define   data_00110_zy_0    2717 //  00110 uV
#define   data_00110_zy_1    3043
#define   data_00110_zy_2    3849
#define   data_00110_zy_3    5777
#define   data_00110_zy_4    9790
#define   data_00110_zy_5    17841
#define   data_00110_zy_6    34268
#define   data_00110_zy_7    66999
#define   data_00110_zy_8    132481
#define   data_00110_zy_9    263533
*/
#define   data_00120_zy_0    771 //  00120 uV
#define   data_00120_zy_1    1542
#define   data_00120_zy_2    3085
#define   data_00120_zy_3    6170
#define   data_00120_zy_4    10525
#define   data_00120_zy_5    19415
#define   data_00120_zy_6    37518
#define   data_00120_zy_7    57331
#define   data_00120_zy_8    114185
#define   data_00120_zy_9    227415
/*
#define   data_00130_zy_0    2761 //  00130 uV
#define   data_00130_zy_1    3172
#define   data_00130_zy_2    4214
#define   data_00130_zy_3    6578
#define   data_00130_zy_4    11364
#define   data_00130_zy_5    20989
#define   data_00130_zy_6    40613
#define   data_00130_zy_7    79638
#define   data_00130_zy_8    157841
#define   data_00130_zy_9    314407
*/
#define   data_00140_zy_0    865 //  00140 uV
#define   data_00140_zy_1    1729
#define   data_00140_zy_2    3458
#define   data_00140_zy_3    6915
#define   data_00140_zy_4    12055
#define   data_00140_zy_5    22555
#define   data_00140_zy_6    43566
#define   data_00140_zy_7    67209
#define   data_00140_zy_8    133940
#define   data_00140_zy_9    267880

#define   data_00150_zy_0    906 //  00150 uV
#define   data_00150_zy_1    1811
#define   data_00150_zy_2    3623
#define   data_00150_zy_3    7246
#define   data_00150_zy_4    12737
#define   data_00150_zy_5    24063
#define   data_00150_zy_6    46298
#define   data_00150_zy_7    70743
#define   data_00150_zy_8    140542
#define   data_00150_zy_9    281086
/*
#define   data_00170_zy_0    2955 //  00170 uV
#define   data_00170_zy_1    3632
#define   data_00170_zy_2    5152
#define   data_00170_zy_3    8360
#define   data_00170_zy_4    14734
#define   data_00170_zy_5    27468
#define   data_00170_zy_6    53138
#define   data_00170_zy_7    103916
#define   data_00170_zy_8    206273
#define   data_00170_zy_9    410869
*/
#define   data_00190_zy_0    950 //  00190 uV
#define   data_00190_zy_1    1900
#define   data_00190_zy_2    3800
#define   data_00190_zy_3    7600
#define   data_00190_zy_4    13222
#define   data_00190_zy_5    24785
#define   data_00190_zy_6    46611
#define   data_00190_zy_7    88605
#define   data_00190_zy_8    176744
#define   data_00190_zy_9    352557
/*
#define   data_00210_zy_0    3410 //  00210 uV
#define   data_00210_zy_1    4221
#define   data_00210_zy_2    6093
#define   data_00210_zy_3    9967
#define   data_00210_zy_4    17760
#define   data_00210_zy_5    33562
#define   data_00210_zy_6    65033
#define   data_00210_zy_7    128114
#define   data_00210_zy_8    254131
#define   data_00210_zy_9    506447
*/
#define   data_00250_zy_0    1017 //  00250 uV
#define   data_00250_zy_1    2033
#define   data_00250_zy_2    4067
#define   data_00250_zy_3    8133
#define   data_00250_zy_4    13951
#define   data_00250_zy_5    25870
#define   data_00250_zy_6    49722
#define   data_00250_zy_7    113582
#define   data_00250_zy_8    226255
#define   data_00250_zy_9    452510
/*
#define   data_00290_zy_0    3913 //  00290 uV
#define   data_00290_zy_1    4802
#define   data_00290_zy_2    7457
#define   data_00290_zy_3    13868
#define   data_00290_zy_4    24149
#define   data_00290_zy_5    46101
#define   data_00290_zy_6    90134
#define   data_00290_zy_7    178057
#define   data_00290_zy_8    354284
#define   data_00290_zy_9    707103
*/
#define   data_00330_zy_0    1350 //  00330 uV
#define   data_00330_zy_1    2700
#define   data_00330_zy_2    5400
#define   data_00330_zy_3    10800
#define   data_00330_zy_4    19295
#define   data_00330_zy_5    36565
#define   data_00330_zy_6    76931
#define   data_00330_zy_7    153538
#define   data_00330_zy_8    306124
#define   data_00330_zy_9    612278
/*
#define   data_00370_zy_0    4192 //  00370 uV
#define   data_00370_zy_1    5847
#define   data_00370_zy_2    9234
#define   data_00370_zy_3    16190
#define   data_00370_zy_4    30210
#define   data_00370_zy_5    58240
#define   data_00370_zy_6    114459
#define   data_00370_zy_7    226824
#define   data_00370_zy_8    451410
#define   data_00370_zy_9    901582
*/


#define   data_0400_zy_0    1621 //  0.400mv
#define   data_0400_zy_1    3242
#define   data_0400_zy_2    7566
#define   data_0400_zy_3    12970
#define   data_0400_zy_4    23805
#define   data_0400_zy_5    45616
#define   data_0400_zy_6    93052
#define   data_0400_zy_7    185726
#define   data_0400_zy_8    371438
#define   data_0400_zy_9    742876

#define   data_0500_zy_0    2036//  0.500mv
#define   data_0500_zy_1    4071//
#define   data_0500_zy_2    8142
#define   data_0500_zy_3    16284
#define   data_0500_zy_4    30461
#define   data_0500_zy_5    58916
#define   data_0500_zy_6    117417
#define   data_0500_zy_7    234630
#define   data_0500_zy_8    468329
#define   data_0500_zy_9    936678

#define   data_0600_zy_0    2485 //  0.600mv
#define   data_0600_zy_1    4970
#define   data_0600_zy_2    9940
#define   data_0600_zy_3    19880
#define   data_0600_zy_4    37630
#define   data_0600_zy_5    70871
#define   data_0600_zy_6    141054
#define   data_0600_zy_7    281738
#define   data_0600_zy_8    563477
#define   data_0600_zy_9    1126954

#define   data_0700_zy_0    2918 //  0.700mv
#define   data_0700_zy_1    5837
#define   data_0700_zy_2    11675
#define   data_0700_zy_3    23350
#define   data_0700_zy_4    44538
#define   data_0700_zy_5    81735
#define   data_0700_zy_6    163221
#define   data_0700_zy_7    325965
#define   data_0700_zy_8    651952
#define   data_0700_zy_9    1303905

#define   data_0800_zy_0    3358 //  0.800mv
#define   data_0800_zy_1    6716
#define   data_0800_zy_2    13433
#define   data_0800_zy_3    26866
#define   data_0800_zy_4    51571
#define   data_0800_zy_5    92794
#define   data_0800_zy_6    185064
#define   data_0800_zy_7    369731
#define   data_0800_zy_8    739065
#define   data_0800_zy_9    1478130

#define   data_0900_zy_0    3574 //  0.900mv
#define   data_0900_zy_1    6773
#define   data_0900_zy_2    13545
#define   data_0900_zy_3    27091
#define   data_0900_zy_4    54183
#define   data_0900_zy_5    107040
#define   data_0900_zy_6    213605
#define   data_0900_zy_7    426730
#define   data_0900_zy_8    853462
#define   data_0900_zy_9    1706924

#define   data_1000_zy_0    4351//  1.000mv
#define   data_1000_zy_1    8063
#define   data_1000_zy_2    15620
#define   data_1000_zy_3    30765
#define   data_1000_zy_4    60850
#define   data_1000_zy_5    121072
#define   data_1000_zy_6    241903
#define   data_1000_zy_7    483809
#define   data_1000_zy_8    966951 ///////////////
#define   data_1000_zy_9    1933905

#define   data_2000_zy_0    8039 //  2.000mv
#define   data_2000_zy_1    15846
#define   data_2000_zy_2    31438
#define   data_2000_zy_3    62529
#define   data_2000_zy_4    124532
#define   data_2000_zy_5    248500
#define   data_2000_zy_6    496872
#define   data_2000_zy_7    993483
#define   data_2000_zy_8    1986967
#define   data_2000_zy_9    ( data_2000_zy_8 * 2)//数据表里写的是FULL3

#define   data_3000_zy_0    12146 //  3.000mv
#define   data_3000_zy_1    24192
#define   data_3000_zy_2    47991
#define   data_3000_zy_3    95683
#define   data_3000_zy_4    190731
#define   data_3000_zy_5    381075
#define   data_3000_zy_6    761886
#define   data_3000_zy_7    1523662
#define   data_3000_zy_8    ( data_3000_zy_7 * 2 )  /////数据表里写的是FULL3
#define   data_3000_zy_9    ( data_3000_zy_8 * 2 ) //数据表里写的是FULL3

#define   data_4000_zy_0    16170 //  4.000mv
#define   data_4000_zy_1    32150
#define   data_4000_zy_2    64018
#define   data_4000_zy_3    127660
#define   data_4000_zy_4    254815
#define   data_4000_zy_5    509242
#define   data_4000_zy_6    1018345
#define   data_4000_zy_7    2036690
#define   data_4000_zy_8    ( data_4000_zy_7 * 2 )  //数据表里写的是FULL3
#define   data_4000_zy_9    ( data_4000_zy_8 * 2 )  //数据表里写的是FULL3

#define   data_5000_zy_0    20644 //  5.000mv
#define   data_5000_zy_1    40650
#define   data_5000_zy_2    80753
#define   data_5000_zy_3    161056
#define   data_5000_zy_4    321650
#define   data_5000_zy_5    642849
#define   data_5000_zy_6    1285618
#define   data_5000_zy_7    ( data_5000_zy_6 * 2 )  //数据表里写的是FULL3
#define   data_5000_zy_8    ( data_5000_zy_7 * 2 )  //数据表里写的是FULL3
#define   data_5000_zy_9    ( data_5000_zy_8 * 2 )  //数据表里写的是FULL3

#define   data_6000_zy_0    24290 //  6.000mv
#define   data_6000_zy_1    48419
#define   data_6000_zy_2    96497
#define   data_6000_zy_3    192611
#define   data_6000_zy_4    384902
#define   data_6000_zy_5    769305
#define   data_6000_zy_6    1538719/////
#define   data_6000_zy_7    ( data_6000_zy_6 * 2 )
#define   data_6000_zy_8    ( data_6000_zy_7 * 2 )
#define   data_6000_zy_9    ( data_6000_zy_8 * 2 )

#define   data_7000_zy_0    28287 //  7.000mv
#define   data_7000_zy_1    56409
#define   data_7000_zy_2    112656
#define   data_7000_zy_3    224988
#define   data_7000_zy_4    449837
#define   data_7000_zy_5    899178
#define   data_7000_zy_6    1798380
#define   data_7000_zy_7    ( data_7000_zy_6 * 2 )
#define   data_7000_zy_8    ( data_7000_zy_7 * 2 )
#define   data_7000_zy_9    ( data_7000_zy_8 * 2 )

#define   data_8000_zy_0    32319 //  8.000mv
#define   data_8000_zy_1    64496
#define   data_8000_zy_2    128673
#define   data_8000_zy_3    257004
#define   data_8000_zy_4    513919
#define   data_8000_zy_5    1027389
#define   data_8000_zy_6    ( data_8000_zy_5 * 2 )
#define   data_8000_zy_7    ( data_8000_zy_6 * 2 )
#define   data_8000_zy_8    ( data_8000_zy_7 * 2 )
#define   data_8000_zy_9    ( data_8000_zy_8 * 2 )

#define   data_9000_zy_0    36410 //  9.000mv
#define   data_9000_zy_1    72575
#define   data_9000_zy_2    144830
#define   data_9000_zy_3    289315
#define   data_9000_zy_4    578360
#define   data_9000_zy_5    1156490
#define   data_9000_zy_6    ( data_9000_zy_5 * 2 )
#define   data_9000_zy_7    ( data_9000_zy_6 * 2 )
#define   data_9000_zy_8    ( data_9000_zy_7 * 2 )
#define   data_9000_zy_9    ( data_9000_zy_8 * 2 )

#define   data_10000_zy_0    40460 //  10.000mv
#define   data_10000_zy_1    80599
#define   data_10000_zy_2    160780
#define   data_10000_zy_3    321321
#define   data_10000_zy_4    642382
#define   data_10000_zy_5    1284390
#define   data_10000_zy_6    ( data_10000_zy_5 * 2 )
#define   data_10000_zy_7    ( data_10000_zy_6 * 2 )
#define   data_10000_zy_8    ( data_10000_zy_7 * 2 )
#define   data_10000_zy_9    ( data_10000_zy_8 * 2 )

#define   data_20000_zy_0    80598 //  20.000mv
#define   data_20000_zy_1    160929
#define   data_20000_zy_2    321434
#define   data_20000_zy_3    641853
#define   data_20000_zy_4    1284485
#define   data_20000_zy_5    ( data_20000_zy_4 * 2 )
#define   data_20000_zy_6    ( data_20000_zy_5 * 2 )
#define   data_20000_zy_7    ( data_20000_zy_6 * 2 )
#define   data_20000_zy_8    ( data_20000_zy_7 * 2 )
#define   data_20000_zy_9    ( data_20000_zy_8 * 2 )

#define   data_30000_zy_0    120050 //  30.000mv
#define   data_30000_zy_1    241506
#define   data_30000_zy_2    482490
#define   data_30000_zy_3    963990
#define   data_30000_zy_4    1928763
#define   data_30000_zy_5    ( data_30000_zy_4 * 2 )
#define   data_30000_zy_6    ( data_30000_zy_5 * 2 )
#define   data_30000_zy_7    ( data_30000_zy_6 * 2 )
#define   data_30000_zy_8    ( data_30000_zy_7 * 2 )
#define   data_30000_zy_9    ( data_30000_zy_8 * 2 )

#define   data_40000_zy_0    160134 //  40.000mv
#define   data_40000_zy_1    340161
#define   data_40000_zy_2    659716
#define   data_40000_zy_3    1285127
#define   data_40000_zy_4    ( data_40000_zy_3 * 2 )
#define   data_40000_zy_5    ( data_40000_zy_4 * 2 )
#define   data_40000_zy_6    ( data_40000_zy_5 * 2 )
#define   data_40000_zy_7    ( data_40000_zy_6 * 2 )
#define   data_40000_zy_8    ( data_40000_zy_7 * 2 )
#define   data_40000_zy_9    ( data_40000_zy_8 * 2 )

#define   data_50000_zy_0    200178 //  50.000mv
#define   data_50000_zy_1    400000
#define   data_50000_zy_2    800000
#define   data_50000_zy_3    1600000
#define   data_50000_zy_4    ( data_50000_zy_3 * 2 )
#define   data_50000_zy_5    ( data_50000_zy_4 * 2 )
#define   data_50000_zy_6    ( data_50000_zy_5 * 2 )
#define   data_50000_zy_7    ( data_50000_zy_6 * 2 )
#define   data_50000_zy_8    ( data_50000_zy_7 * 2 )
#define   data_50000_zy_9    ( data_50000_zy_8 * 2 )

#define   data_100000_zy_0    400980 //  100.000mv
#define   data_100000_zy_1    ( data_100000_zy_0 * 2 )
#define   data_100000_zy_2    ( data_100000_zy_1 * 2 )
#define   data_100000_zy_3    ( data_100000_zy_2 * 2 )
#define   data_100000_zy_4    ( data_100000_zy_3 * 2 )
#define   data_100000_zy_5    ( data_100000_zy_4 * 2 )
#define   data_100000_zy_6    ( data_100000_zy_5 * 2 )
#define   data_100000_zy_7    ( data_100000_zy_6 * 2 )
#define   data_100000_zy_8    ( data_100000_zy_7 * 2 )
#define   data_100000_zy_9    ( data_100000_zy_8 * 2 )

#define   data_200000_zy_0    802515 //  200.000mv
#define   data_200000_zy_1    ( data_200000_zy_0 * 2 )
#define   data_200000_zy_2    ( data_200000_zy_1 * 2 )
#define   data_200000_zy_3    ( data_200000_zy_2 * 2 )
#define   data_200000_zy_4    ( data_200000_zy_3 * 2 )
#define   data_200000_zy_5    ( data_200000_zy_4 * 2 )
#define   data_200000_zy_6    ( data_200000_zy_5 * 2 )
#define   data_200000_zy_7    ( data_200000_zy_6 * 2 )
#define   data_200000_zy_8    ( data_200000_zy_7 * 2 )
#define   data_200000_zy_9    ( data_200000_zy_8 * 2 )

#define   data_300000_zy_0    1202710 //  300.000mv
#define   data_300000_zy_1    ( data_300000_zy_0 * 2 )
#define   data_300000_zy_2    ( data_300000_zy_1 * 2 )
#define   data_300000_zy_3    ( data_300000_zy_2 * 2 )
#define   data_300000_zy_4    ( data_300000_zy_3 * 2 )
#define   data_300000_zy_5    ( data_300000_zy_4 * 2 )
#define   data_300000_zy_6    ( data_300000_zy_5 * 2 )
#define   data_300000_zy_7    ( data_300000_zy_6 * 2 )
#define   data_300000_zy_8    ( data_300000_zy_7 * 2 )
#define   data_300000_zy_9    ( data_300000_zy_8 * 2 )

#define   data_400000_zy_0    1604377 //  400.000mv
#define   data_400000_zy_1    ( data_400000_zy_0 * 2 )
#define   data_400000_zy_2    ( data_400000_zy_1 * 2 )
#define   data_400000_zy_3    ( data_400000_zy_2 * 2 )
#define   data_400000_zy_4    ( data_400000_zy_3 * 2 )
#define   data_400000_zy_5    ( data_400000_zy_4 * 2 )
#define   data_400000_zy_6    ( data_400000_zy_5 * 2 )
#define   data_400000_zy_7    ( data_400000_zy_6 * 2 )
#define   data_400000_zy_8    ( data_400000_zy_7 * 2 )
#define   data_400000_zy_9    ( data_400000_zy_8 * 2 )

#define   data_500000_zy_0    2005197 //  500.000mv
#define   data_500000_zy_1    ( data_500000_zy_0 * 2 )
#define   data_500000_zy_2    ( data_500000_zy_1 * 2 )
#define   data_500000_zy_3    ( data_500000_zy_2 * 2 )
#define   data_500000_zy_4    ( data_500000_zy_3 * 2 )
#define   data_500000_zy_5    ( data_500000_zy_4 * 2 )
#define   data_500000_zy_6    ( data_500000_zy_5 * 2 )
#define   data_500000_zy_7    ( data_500000_zy_6 * 2 )
#define   data_500000_zy_8    ( data_500000_zy_7 * 2 )
#define   data_500000_zy_9    ( data_500000_zy_8 * 2 )

#define   data_550000_zy_0    2220000 //  550.000mv
#define   data_550000_zy_1    ( data_550000_zy_0 * 2 )
#define   data_550000_zy_2    ( data_550000_zy_1 * 2 )
#define   data_550000_zy_3    ( data_550000_zy_2 * 2 )
#define   data_550000_zy_4    ( data_550000_zy_3 * 2 )
#define   data_550000_zy_5    ( data_550000_zy_4 * 2 )
#define   data_550000_zy_6    ( data_550000_zy_5 * 2 )
#define   data_550000_zy_7    ( data_550000_zy_6 * 2 )
#define   data_550000_zy_8    ( data_550000_zy_7 * 2 )
#define   data_550000_zy_9    ( data_550000_zy_8 * 2 )

/////////////////////////////////////////////////////////////////////////////////////////
//电流部分  电流表读数   没有电流修正的程序读出的电流，单位mA

#define   data_current_000    3000  //屏显 0.000 mA
#define   data_current_010    3221
#define   data_current_020    6856
#define   data_current_030    9485
#define   data_current_040    12114
#define   data_current_050    14743
#define   data_current_060    17372
#define   data_current_070    20001
#define   data_current_080    23630
#define   data_current_090    25259
#define   data_current_100    27888
#define   data_current_200    55776


