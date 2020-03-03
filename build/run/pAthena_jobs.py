# TO RUN: python -i pAthena_job_files.py ?

#! /usr/bin/env python

import os
import commands

# Put the list of datasets you want to run over here (remove the '/' from the end). 
inDSs = [
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s2997_r9064_r8996_tid10585483_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s3126_r9364_r9315_tid11182601_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s3126_r9364_r9315_tid11182673_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s3126_d1480_d1471_r10681_r10682_tid15260503_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_a875_r10201_r10210_tid12863977_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_a875_r9364_r9315_tid13193232_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s3126_r10201_r10210_tid13012111_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_s3126_r10239_r10210_tid13027897_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_r10201_r10210_tid13012093_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_r10239_r10210_tid13027876_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s3126_r10684_r11331_tid17749613_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s3126_r11634_tid19504148_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_a875_r11487_r9315_tid18581520_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_a875_r9364_r9315_tid13193222_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_r10416_r10401_tid13655778_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_r10724_r10726_tid14838680_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s3126_r10652_tid14764413_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s3126_r10684_r11328_tid17738063_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_r10530_r10541_tid14316749_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_s3126_s3136_r10724_r10726_tid15260020_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s3126_r10684_tid15453889_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.digit.AOD.e3601_s3126_r11478_d1523_r11479_tid18530523_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_a875_r10724_r10726_tid16294178_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e3601_e5984_a875_r10724_r10726_tid16294213_00',
##    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.digit.AOD.e3601_s3126_r11234_d1505_r11235_tid17123474_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3460_r11453_r10726_tid18972170_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3459_r11452_r10726_tid18972138_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3458_r11451_r10726_tid18972109_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3126_r11450_r10726_tid18972212_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3126_r11591_r10726_tid19119295_00',
    'mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.merge.AOD.e7623_e5984_s3126_r11590_r10726_tid19119260_00',
    ]

command = "pathena --inDS "

for i,inDS in enumerate(inDSs):
    command += "%s/,"%(inDS)

command = command[:-1] # take off the last comma
command += " --mergeOutput --outDS user.aburke.testing_new_pAthena_jobs_command ~/public/Masters/build/x86_64-centos7-gcc62-opt/jobOptions/DiTau/DiTauAlgJobOptions.py"
print command
os.system(command)
print "\n"
