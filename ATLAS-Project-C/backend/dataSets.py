# file to hold variables with info about the data

# total luminosity of the real data in inverse femtobarns
totRealLum = 10.064

# list of directories to check for .root files
dirs = [
        "/data/ATLAS/2lep/Data/",
        "/data/ATLAS/2lep/MC/",
        "/data/ATLAS/1lep/Data/",
        "/data/ATLAS/1lep/MC/",
        "/data/ATLAS/GamGam/Data/",
        "/data/ATLAS/GamGam/MC/",
        "/data/ATLAS/1tau/Data/",
        "/data/ATLAS/1tau/MC/",
        "/data/ATLAS/2tau/Data/",
        "/data/ATLAS/2tau/MC/",
        "/data/ATLAS/1lep1tau/Data/",
        "/data/ATLAS/1lep1tau/MC/",
        "/data/ATLAS/1fatjet1lep/Data/",
        "/data/ATLAS/1fatjet1lep/MC/",
]

# list of keys which correspond to real datasets
realList = [
        "A", "B", "C", "D",
        "A_1lep", "B_1lep", "C_1lep", "D_1lep",
        "A_yy", "B_yy", "C_yy", "D_yy",
        "A_1tau0lep", "B_1tau0lep", "C_1tau0lep", "D_1tau0lep",
        "A_2tau", "B_2tau", "C_2tau", "D_2tau",
        "A_1tau1lep", "B_1tau1lep", "C_1tau1lep", "D_1tau1lep"
        ]

# dictionary of combinations of keys to be analysed together when a single string is
# inputted by the user
dataCombos = {
        # real data
        "2lep": ["A", "B", "C", "D"],
        "1lep": ["A_1lep", "B_1lep", "C_1lep", "D_1lep"],
        "yy": ["A_yy", "B_yy", "C_yy", "D_yy"],
        "1tau0lep": ["A_1tau0lep", "B_1tau0lep", "C_1tau0lep", "D_1tau0lep"],
        "1tau1lep": ["A_1tau1lep", "B_1tau1lep", "C_1tau1lep", "D_1tau1lep"],
        "2tau": ["A_2tau", "B_2tau", "C_2tau", "D_2tau"],

        # W
        "Wplus": ["Wplusenu","Wplusmunu","Wplustaunu"],
        "Wminus": ["Wminusenu","Wminusmunu","Wminustaunu"],
        "Wplus_2lep": ["Wplusenu_2lep","Wplusmunu_2lep","Wplustaunu_2lep"],
        "Wminus_2lep": ["Wminusenu_2lep","Wminusmunu_2lep","Wminustaunu_2lep"],

        # Z
        "Z_1lep": ["Zee_1lep","Zmumu_1lep","Ztautau_1lep"],
        "Z": ["Zee","Zmumu","Ztautau"],

        # Dominant Higgs -> 4 lepton production
        "H": ["ZH125_ZZ4lep", "WH125_ZZ4lep", "VBFH125_ZZ4lep", "ggH125_ZZ4lep"],
        "Hyy": ["ttHyy", "ZHyy", "WHyy", "VBFHyy", "ggHyy"]
        }

# dictionary to identify data by short names rather than long file names, contains
# luminosities for each dataset in femtobarns
dataSets = {
    # from /data/OpenData directory

    # diphoton data
    "A_yy": "dataA_yy.root",
    "B_yy": "dataB_yy.root",
    "C_yy": "dataC_yy.root",
    "D_yy": "dataD_yy.root",

    # diphoton MC
    "ttHyy": "mc15_13TeV.341081.aMcAtNloHerwigppEG_UEEE5_CTEQ6L1_CT10ME_ttH125_gamgam.gamgam_raw.root",
    "ggHyy": "mc15_13TeV.343981.PwPy8EG_NNLOPS_nnlo_30_ggH125_gamgam.gamgam_raw.root",
    "VBFHyy": "mc15_13TeV.345041.PwPy8EG_NNPDF30_AZNLOCTEQ6L1_VBFH125_gamgam.gamgam_raw.root",
    "WHyy": "mc15_13TeV.345318.PwPy8EG_NNPDF30_AZNLO_WpH125J_Hyy_Wincl_MINLO.gamgam_raw.root",
    "ZHyy": "mc15_13TeV.345319.PwPy8EG_NNPDF30_AZNLO_ZH125J_Hyy_Zincl_MINLO.gamgam_raw.root",

    # 2 lepton measured data
    "A": "dataA_2lep.root",
    "B": "dataB_2lep.root",
    "C": "dataC_2lep.root",
    "D": "dataD_2lep.root",

    # Monte Carlo 2 lepton
    "ZH125_ZZ4lep": "mc15_13TeV.341947.Py8EG_A14NNPDF23LO_ZH125_ZZ4l.2lep_raw.root",
    "WH125_ZZ4lep": "mc15_13TeV.341964.Py8EG_A14NNPDF23LO_WH125_ZZ4l.2lep_raw.root",
    "VBFH125_ZZ4lep": "mc15_13TeV.344235.PwPy8EG_NNPDF30_AZNLOCTEQ6L1_VBFH125_ZZ4lep_notau.2lep_raw.root",
    "ggH125_ZZ4lep": "mc15_13TeV.345060.PwPy8EG_NNLOPS_nnlo_30_ggH125_ZZ4l.2lep_raw.root",
    "Zee": "mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.2lep_raw.root",
    "Zmumu": "mc15_13TeV.361107.PwPy8EG_AZNLOCTEQ6L1_Zmumu.2lep_raw.root",
    "Ztautau": "mc15_13TeV.361108.PwPy8EG_AZNLOCTEQ6L1_Ztautau.2lep_raw.root",
    "ZZllll": "mc15_13TeV.361603.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZllll_mll4.2lep_raw.root",

    # redundant simulations, may delete
#    "Zmumu0_70CVetoBVeto": "mc15_13TeV.364100.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_CVetoBVeto.2lep_raw.root",
#    "Zmumu0_70CFilterBVeto": "mc15_13TeV.364101.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_CFilterBVeto.2lep_raw.root",
#    "Zmumu0_70BFilter": "mc15_13TeV.364102.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_BFilter.2lep_raw.root",
#    "Zmumu70_140CVetoBVeto": "mc15_13TeV.364103.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_CVetoBVeto.2lep_raw.root",
#    "Zmumu70_140CFilterBVeto": "mc15_13TeV.364104.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_CFilterBVeto.2lep_raw.root",
#    "Zmumu70_140BFilter": "mc15_13TeV.364105.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_BFilter.2lep_raw.root",
#    "Zmumu140_280CVetoBVeto": "mc15_13TeV.364106.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_CVetoBVeto.2lep_raw.root",
#    "Zmumu140_280CFilterBVeto": "mc15_13TeV.364107.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_CFilterBVeto.2lep_raw.root",
#    "Zmumu140_280BFilter": "mc15_13TeV.364108.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_BFilter.2lep_raw.root",
#    "Zmumu280_500CVetoBVeto": "mc15_13TeV.364109.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_CVetoBVeto.2lep_raw.root",
#    "Zmumu280_500CFilterBVeto": "mc15_13TeV.364110.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_CFilterBVeto.2lep_raw.root",
#    "Zmumu280_500BFilter": "mc15_13TeV.364111.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_BFilter.2lep_raw.root",
#    "Zmumu500_1000": "mc15_13TeV.364112.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV500_1000.2lep_raw.root",
#    "Zmumu1000": "mc15_13TeV.364113.Sh_221_NNPDF30NNLO_Zmumu_MAXHTPTV1000_E_CMS.2lep_raw.root",
#    "Zee0_70CVetoBVeto": "mc15_13TeV.364114.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_CVetoBVeto.2lep_raw.root",
#    "Zee0_70CFilterBVeto": "mc15_13TeV.364115.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_CFilterBVeto.2lep_raw.root",
#    "Zee0_70BFilter": "mc15_13TeV.364116.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_BFilter.2lep_raw.root",
#    "Zee70_140CVetoBVeto": "mc15_13TeV.364117.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_CVetoBVeto.2lep_raw.root",
#    "Zee70_140CFilterBVeto": "mc15_13TeV.364118.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_CFilterBVeto.2lep_raw.root",
#    "Zee70_140BFilter": "mc15_13TeV.364119.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_BFilter.2lep_raw.root",
#    "Zee140_280CVetoBVeto": "mc15_13TeV.364120.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CVetoBVeto.2lep_raw.root",
#    "Zee140_280CFilterBVeto": "mc15_13TeV.364121.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CFilterBVeto.2lep_raw.root",
#    "Zee140_280BFilter": "mc15_13TeV.364122.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_BFilter.2lep_raw.root",
#    "Zee280_500CVetoBVeto": "mc15_13TeV.364123.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CVetoBVeto.2lep_raw.root",
#    "Zee280_500CFilterBVeto": "mc15_13TeV.364124.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CFilterBVeto.2lep_raw.root",
#    "Zee280_500BFilter": "mc15_13TeV.364125.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_BFilter.2lep_raw.root",
#    "Zee500_1000": "mc15_13TeV.364126.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV500_1000.2lep_raw.root",
#    "Zee1000": "mc15_13TeV.364127.Sh_221_NNPDF30NNLO_Zee_MAXHTPTV1000_E_CMS.2lep_raw.root",
#    "Ztautau0_70CVetoBVeto": "mc15_13TeV.364128.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CVetoBVeto.2lep_raw.root",
#    "Ztautau0_70CFilterBVeto": "mc15_13TeV.364129.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CFilterBVeto.2lep_raw.root",
#    "Ztautau0_70BFilter": "mc15_13TeV.364130.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_BFilter.2lep_raw.root",
#    "Ztautau70_140CVetoBVeto": "mc15_13TeV.364131.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CVetoBVeto.2lep_raw.root",
#    "Ztautau70_140CFilterBVeto": "mc15_13TeV.364132.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CFilterBVeto.2lep_raw.root",
#    "Ztautau70_140BFilter": "mc15_13TeV.364133.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_BFilter.2lep_raw.root",
#    "Ztautau140_280CVetoBVeto": "mc15_13TeV.364134.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_CVetoBVeto.2lep_raw.root",
#    "Ztautau140_280CFilterBVeto": "mc15_13TeV.364135.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_CFilterBVeto.2lep_raw.root",
#    "Ztautau140_280BFilter": "mc15_13TeV.364136.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_BFilter.2lep_raw.root",
#    "Ztautau280_500CVetoBVeto": "mc15_13TeV.364137.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_CVetoBVeto.2lep_raw.root",
#    "Ztautau280_500CFilterBVeto": "mc15_13TeV.364138.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_CFilterBVeto.2lep_raw.root",
#    "Ztautau280_500BFilter": "mc15_13TeV.364139.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_BFilter.2lep_raw.root",
#    "Ztautau500_1000": "mc15_13TeV.364140.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV500_1000.2lep_raw.root",
#    "Ztautau1000": "mc15_13TeV.364141.Sh_221_NNPDF30NNLO_Ztautau_MAXHTPTV1000_E_CMS.2lep_raw.root",
    # end of redundant simulations
    

    # ~/data/ directories
    # more 2 lepton MC
    "Zprime2000ee": "mc15_13TeV.301215.Py8EG_A14NNPDF23LO_Zprime_NoInt_ee_E6Chi2000.2lep_raw.root",
    "Zprime3000ee": "mc15_13TeV.301216.Py8EG_A14NNPDF23LO_Zprime_NoInt_ee_E6Chi3000.2lep_raw.root",
    "Zprime4000ee": "mc15_13TeV.301217.Py8EG_A14NNPDF23LO_Zprime_NoInt_ee_E6Chi4000.2lep_raw.root",
    "Zprime5000ee": "mc15_13TeV.301218.Py8EG_A14NNPDF23LO_Zprime_NoInt_ee_E6Chi5000.2lep_raw.root",
    "Zprime2000mumu": "mc15_13TeV.301220.Py8EG_A14NNPDF23LO_Zprime_NoInt_mumu_E6Chi2000.2lep_raw.root",
    "Zprime3000mumu": "mc15_13TeV.301221.Py8EG_A14NNPDF23LO_Zprime_NoInt_mumu_E6Chi3000.2lep_raw.root",
    "Zprime4000mumu": "mc15_13TeV.301222.Py8EG_A14NNPDF23LO_Zprime_NoInt_mumu_E6Chi4000.2lep_raw.root",
    "Zprime400tt": "mc15_13TeV.301322.Py8EG_A14NNPDF23LO_zprime400_tt.2lep_raw.root",
    "Zprime500tt": "mc15_13TeV.301323.Py8EG_A14NNPDF23LO_zprime500_tt.2lep_raw.root",
    "Zprime750tt": "mc15_13TeV.301324.Py8EG_A14NNPDF23LO_zprime750_tt.2lep_raw.root",
    "Zprime1000tt": "mc15_13TeV.301325.Py8EG_A14NNPDF23LO_zprime1000_tt.2lep_raw.root",
    "Zprime1250tt": "mc15_13TeV.301326.Py8EG_A14NNPDF23LO_zprime1250_tt.2lep_raw.root",
    "Zprime1500tt": "mc15_13TeV.301327.Py8EG_A14NNPDF23LO_zprime1500_tt.2lep_raw.root",
    "Zprime1750tt": "mc15_13TeV.301328.Py8EG_A14NNPDF23LO_zprime1750_tt.2lep_raw.root",
    "Zprime2000tt": "mc15_13TeV.301329.Py8EG_A14NNPDF23LO_zprime2000_tt.2lep_raw.root",
    "Zprime2250tt": "mc15_13TeV.301330.Py8EG_A14NNPDF23LO_zprime2250_tt.2lep_raw.root",
    "Zprime2500tt": "mc15_13TeV.301331.Py8EG_A14NNPDF23LO_zprime2500_tt.2lep_raw.root",
    "Zprime2750tt": "mc15_13TeV.301332.Py8EG_A14NNPDF23LO_zprime2750_tt.2lep_raw.root",
    "Zprime3000tt": "mc15_13TeV.301333.Py8EG_A14NNPDF23LO_zprime3000_tt.2lep_raw.root",
    "Zprime400tt": "mc15_13TeV.302681.MGPy8EG_A14NNPDF23LO_LRSM_WR2400_NR1800.2lep_raw.root",
    "Zprime500tt": "mc15_13TeV.302687.MGPy8EG_A14NNPDF23LO_LRSM_WR3000_NR1500.2lep_raw.root",
    "Zprime750tt": "mc15_13TeV.302708.MGPy8EG_A14NNPDF23LO_LRSM_WR4200_NR2100.2lep_raw.root",
    "Graviton1000": "mc15_13TeV.303329.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m1000.2lep_raw.root",
    "Graviton2000": "mc15_13TeV.303334.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m2000.2lep_raw.root",
    "MonoZ10": "mc15_13TeV.303511.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM10.2lep_raw.root",
    "MonoZ100": "mc15_13TeV.303512.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM100.2lep_raw.root",
    "MonoZ300": "mc15_13TeV.303513.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM300.2lep_raw.root",
    "MonoZ2000": "mc15_13TeV.303514.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM200.2lep_raw.root",
    # gluino data giving problems, should not need these anyway
#    "gluino1200DM600": "mc15_13TeV.305550.Py8EG_A14NNPDF23LO_Gee_01_750.2lep_raw.root",
#    "gluino1200DM600_": "mc15_13TeV.305553.Py8EG_A14NNPDF23LO_Gee_01_1000.2lep_raw.root",
#    "gluino1200DM600": "mc15_13TeV.305556.Py8EG_A14NNPDF23LO_Gee_01_2000.2lep_raw.root",
#    "gluino1200DM600_": "mc15_13TeV.305559.Py8EG_A14NNPDF23LO_Gee_01_3000.2lep_raw.root",
#    "gluino1200DM600": "mc15_13TeV.305562.Py8EG_A14NNPDF23LO_Gee_01_4000.2lep_raw.root",
#    "gluino1200DM600_": "mc15_13TeV.305568.Py8EG_A14NNPDF23LO_Gmumu_01_750.2lep_raw.root",
#    "gluino1200DM600": "mc15_13TeV.305571.Py8EG_A14NNPDF23LO_Gmumu_01_1000.2lep_raw.root",
#    "gluino1200DM600_": "mc15_13TeV.305574.Py8EG_A14NNPDF23LO_Gmumu_01_2000.2lep_raw.root",
#    "gluino1200DM600": "mc15_13TeV.305577.Py8EG_A14NNPDF23LO_Gmumu_01_3000.2lep_raw.root",
#    "gluino1200DM600_": "mc15_13TeV.305580.Py8EG_A14NNPDF23LO_Gmumu_01_4000.2lep_raw.root",
    "MonoZ500": "mc15_13TeV.305710.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM500.2lep_raw.root",
    "MonoZ700": "mc15_13TeV.305711.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM700.2lep_raw.root",
    "MonoZ200": "mc15_13TeV.306085.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM200.2lep_raw.root",
    "MonoZ400": "mc15_13TeV.306093.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM400.2lep_raw.root",
    "MonoZ600": "mc15_13TeV.306103.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM600.2lep_raw.root",
    "MonoZ800": "mc15_13TeV.306109.MGPy8EG_A14NNPDF30_dmV_Zll_MET40_DM1_MM800.2lep_raw.root",
    "Graviton200": "mc15_13TeV.307431.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m0200.2lep_raw.root",
    "Graviton500": "mc15_13TeV.307434.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m0500.2lep_raw.root",
    "Graviton1500": "mc15_13TeV.307439.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m1500.2lep_raw.root",
    "Graviton1500": "mc15_13TeV.309070.MGPy8EG_A14NNPDF23LO_LRSM_WR5000_NR2500.2lep_raw.root",
    "ggHtautau_2lep": "mc15_13TeV.341122.PwPy8EG_CT10_AZNLOCTEQ6L1_ggH125_tautaull.2lep_raw.root",
    "VBFHtautau_2lep": "mc15_13TeV.341155.PwPy8EG_CT10_AZNLOCTEQ6L1_VBFH125_tautaull.2lep_raw.root",
    "VBFH125_WW2lep": "mc15_13TeV.345323.PwPy8EG_NNPDF30_AZNLOCTEQ6L1_VBFH125_WWlvlv.2lep_raw.root",
    "ggH125_WW2lep": "mc15_13TeV.345324.PwPy8EG_NNLOPS_NN30_ggH125_WWlvlv_EF_15_5.2lep_raw.root",
    "WH125_qqWW2lep": "mc15_13TeV.345325.PwPy8EG_NNPDF3_AZNLO_WpH125J_MINLO_qqWWlvlv.2lep_raw.root",
    "WH125_lvWW2lep": "mc15_13TeV.345327.PwPy8EG_NNPDF3_AZNLO_WpH125J_MINLO_lvWWlvlv.2lep_raw.root",
    "ZH125_qqWW2lep": "mc15_13TeV.345336.PwPy8EG_NNPDF3_AZNLO_ZH125J_MINLO_qqWWlvlv.2lep_raw.root",
    "ZH125_llWW2lep": "mc15_13TeV.345337.PwPy8EG_NNPDF3_AZNLO_ZH125J_MINLO_llWWlvlv.2lep_raw.root",
    "ZH125_vvWW2lep": "mc15_13TeV.345445.PwPy8EG_NNPDF3_AZNLO_ZH125J_MINLO_vvWWlvlv.2lep_raw.root",
    "VBFH125_WW2lep": "mc15_13TeV.361063.Sh_CT10_llll.2lep_raw.root",
    "ggH125_WW2lep": "mc15_13TeV.361064.Sh_CT10_lllvSFMinus.2lep_raw.root",
    "WH125_qqWW2lep": "mc15_13TeV.361065.Sh_CT10_lllvOFMinus.2lep_raw.root",
    "WH125_lvWW2lep": "mc15_13TeV.361066.Sh_CT10_lllvSFPlus.2lep_raw.root",
    "ZH125_qqWW2lep": "mc15_13TeV.361067.Sh_CT10_lllvOFPlus.2lep_raw.root",
    "ZH125_llWW2lep": "mc15_13TeV.361068.Sh_CT10_llvv.2lep_raw.root",
    "Wplusenu_2lep": "mc15_13TeV.361100.PwPy8EG_AZNLOCTEQ6L1_Wplusenu.2lep_raw.root",
    "Wplusmunu_2lep": "mc15_13TeV.361101.PwPy8EG_AZNLOCTEQ6L1_Wplusmunu.2lep_raw.root",
    "Wplustaunu_2lep": "mc15_13TeV.361102.PwPy8EG_AZNLOCTEQ6L1_Wplustaunu.2lep_raw.root",
    "Wminusenu_2lep": "mc15_13TeV.361103.PwPy8EG_AZNLOCTEQ6L1_Wminusenu.2lep_raw.root",
    "Wminusmunu_2lep": "mc15_13TeV.361104.PwPy8EG_AZNLOCTEQ6L1_Wminusmunu.2lep_raw.root",
    "Wminustaunu_2lep": "mc15_13TeV.361105.PwPy8EG_AZNLOCTEQ6L1_Wminustaunu.2lep_raw.root",
    "WWlvlv": "mc15_13TeV.361600.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvlv.2lep_raw.root",
    "WZlvll": "mc15_13TeV.361601.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WZlvll_mll4.2lep_raw.root",
    "WZlvvv": "mc15_13TeV.361602.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WZlvvv_mll4.2lep_raw.root",
    "ZZllll": "mc15_13TeV.361603.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZllll_mll4.2lep_raw.root",
    "ZZvvll": "mc15_13TeV.361604.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZvvll_mll4.2lep_raw.root",
    "WWlvqq": "mc15_13TeV.361606.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvqq.2lep_raw.root",
    "WZqqll": "mc15_13TeV.361607.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WZqqll_mll20.2lep_raw.root",
    "WZlvqq": "mc15_13TeV.361609.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_WZlvqq_mqq20.2lep_raw.root",
    "ZZqqll": "mc15_13TeV.361610.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZqqll_mqq20mll20.2lep_raw.root",
    "ZqqZll": "mc15_13TeV.363356.Sh_221_NNPDF30NNLO_ZqqZll.2lep_raw.root",
    "WqqZll": "mc15_13TeV.363358.Sh_221_NNPDF30NNLO_WqqZll.2lep_raw.root",
    "WqqWlv": "mc15_13TeV.363359.Sh_221_NNPDF30NNLO_WpqqWmlv.2lep_raw.root",
    "WlvWqq": "mc15_13TeV.363360.Sh_221_NNPDF30NNLO_WplvWmqq.2lep_raw.root",
    "WlvZqq": "mc15_13TeV.363489.Sh_221_NNPDF30NNLO_WlvZqq.2lep_raw.root",
    "ZllZll": "mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.2lep_raw.root",
    "WlvZll": "mc15_13TeV.363491.Sh_221_NNPDF30NNLO_lllv.2lep_raw.root",
    "llvv": "mc15_13TeV.363492.Sh_221_NNPDF30NNLO_llvv.2lep_raw.root",
    "WlvZvv": "mc15_13TeV.363493.Sh_221_NNPDF30NNLO_lvvv.2lep_raw.root",
    "single_top_tchan": "mc15_13TeV.363494.Sh_221_NNPDF30NNLO_vvvv.2lep_raw.root",
    "single_antitop_tchan": "mc15_13TeV.364250.Sh_222_NNPDF30NNLO_llll.2lep_raw.root",
    "ttbar_lep": "mc15_13TeV.410000.PwPyEG_P2012_ttbar_hdamp172p5_nonallhad.2lep_raw.root",
    "single_top_tchan_2lep": "mc15_13TeV.410011.PwPyEG_P2012_singletop_tchan_lept_top.2lep_raw.root",
    "single_antitop_tchan_2lep": "mc15_13TeV.410012.PwPyEG_P2012_singletop_tchan_lept_antitop.2lep_raw.root",
    "single_top_wtchan_2lep": "mc15_13TeV.410013.PwPyEG_P2012_Wt_inclusive_top.2lep_raw.root",
    "single_antitop_wtchan_2lep": "mc15_13TeV.410014.PwPyEG_P2012_Wt_inclusive_antitop.2lep_raw.root",
    "single_top_schan_2lep": "mc15_13TeV.410025.PwPyEG_P2012_SingleTopSchan_noAllHad_top.2lep_raw.root",
    "single_antitop_schan_2lep": "mc15_13TeV.410026.PwPyEG_P2012_SingleTopSchan_noAllHad_antitop.2lep_raw.root",
    "single_antitop_tchan_2lep": "mc15_13TeV.410155.aMcAtNloPy8EG_MEN30NLO_A14N23LO_ttW.2lep_raw.root",
    "single_antitop_wtchan_2lep": "mc15_13TeV.410218.aMcAtNloPy8EG_MEN30NLO_A14N23LO_ttee.2lep_raw.root",
    "single_antitop_schan_2lep": "mc15_13TeV.410219.aMcAtNloPy8EG_MEN30NLO_A14N23LO_ttmumu.2lep_raw.root",

    # 1 lepton data
    "A_1lep": "dataA_1lep.root",
    "B_1lep": "dataB_1lep.root",
    "C_1lep": "dataC_1lep.root",
    "D_1lep": "dataD_1lep.root",

    # 1 lepton MC
    "Wplusenu": "mc15_13TeV.361100.PwPy8EG_AZNLOCTEQ6L1_Wplusenu.1lep_raw.root",
    "Wplusmunu": "mc15_13TeV.361101.PwPy8EG_AZNLOCTEQ6L1_Wplusmunu.1lep_raw.root",
    "Wplustaunu": "mc15_13TeV.361102.PwPy8EG_AZNLOCTEQ6L1_Wplustaunu.1lep_raw.root",
    "Wminusenu": "mc15_13TeV.361103.PwPy8EG_AZNLOCTEQ6L1_Wminusenu.1lep_raw.root",
    "Wminusmunu": "mc15_13TeV.361104.PwPy8EG_AZNLOCTEQ6L1_Wminusmunu.1lep_raw.root",
    "Wminustaunu": "mc15_13TeV.361105.PwPy8EG_AZNLOCTEQ6L1_Wminustaunu.1lep_raw.root",
    "Zee_1lep": "mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.1lep_raw.root",
    "Zmumu_1lep": "mc15_13TeV.361107.PwPy8EG_AZNLOCTEQ6L1_Zmumu.1lep_raw.root",
    "Ztautau_1lep": "mc15_13TeV.361108.PwPy8EG_AZNLOCTEQ6L1_Ztautau.1lep_raw.root",
    "WlvWqq": "mc15_13TeV.363360.Sh_221_NNPDF30NNLO_WplvWmqq.1lep_raw.root",
    "WlvZqq": "mc15_13TeV.363489.Sh_221_NNPDF30NNLO_WlvZqq.1lep_raw.root",
    "WlvZvv": "mc15_13TeV.363493.Sh_221_NNPDF30NNLO_lvvv.1lep_raw.root",
    "ttbar_lep_1lep": "mc15_13TeV.410000.PwPyEG_P2012_ttbar_hdamp172p5_nonallhad.1lep_raw.root",
    "single_top_tchan": "mc15_13TeV.410011.PwPyEG_P2012_singletop_tchan_lept_top.1lep_raw.root",
    "single_antitop_tchan": "mc15_13TeV.410012.PwPyEG_P2012_singletop_tchan_lept_antitop.1lep_raw.root",
    "single_top_wtchan": "mc15_13TeV.410013.PwPyEG_P2012_Wt_inclusive_top.1lep_raw.root",
    "single_antitop_wtchan": "mc15_13TeV.410014.PwPyEG_P2012_Wt_inclusive_antitop.1lep_raw.root",
    "single_top_schan": "mc15_13TeV.410025.PwPyEG_P2012_SingleTopSchan_noAllHad_top.1lep_raw.root",
    "single_antitop_schan": "mc15_13TeV.410026.PwPyEG_P2012_SingleTopSchan_noAllHad_antitop.1lep_raw.root",

    
    # 1 tau 0 lep data
    "A_1tau0lep": "dataA_1tau0lep.root",
    "B_1tau0lep": "dataB_1tau0lep.root",
    "C_1tau0lep": "dataC_1tau0lep.root",
    "D_1tau0lep": "dataD_1tau0lep.root",

    # 1 tau 0 lep MC
    # no infofile entry for Z or top
    "Wplustaunu": "mc15_13TeV.361102.PwPy8EG_AZNLOCTEQ6L1_Wplustaunu.1tau_raw.root",
    "Wminustaunu": "mc15_13TeV.361105.PwPy8EG_AZNLOCTEQ6L1_Wminustaunu.1tau_raw.root",
#    "Zee": "mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.1tau_raw.root",
#    "Zmumu": "mc15_13TeV.361107.PwPy8EG_AZNLOCTEQ6L1_Zmumu.1tau_raw.root",
#    "single_top_schan_1lep1tau": "mc15_13TeV.410025.PwPyEG_P2012_SingleTopSchan_noAllHad_top.1tau_raw.root",


    # 2 tau data
    "A_2tau": "dataA_2tau.root",
    "B_2tau": "dataB_2tau.root",
    "C_2tau": "dataC_2tau.root",
    "D_2tau": "dataD_2tau.root",

    # 2 tau MC
    "ggHtautau_2tau": "mc15_13TeV.341124.PwPy8EG_CT10_AZNLOCTEQ6L1_ggH125_tautauhh.2tau_raw.root",
    "VBFHtautau_2tau": "mc15_13TeV.341157.PwPy8EG_CT10_AZNLOCTEQ6L1_VBFH125_tautauhh.2tau_raw.root",
    "Ztautau_2tau": "mc15_13TeV.361108.PwPy8EG_AZNLOCTEQ6L1_Ztautau.2tau_raw.root",


    # 1 lep 1 tau data
    "A_1tau1lep":"dataA_1tau1lep.root",
    "B_1tau1lep":"dataB_1tau1lep.root",
    "C_1tau1lep":"dataC_1tau1lep.root",
    "D_1tau1lep":"dataD_1tau1lep.root",

    # 1 lep 1 tau MC
    # commented datasets not in infofile
    "ggHtautau_1tau1lep": "mc15_13TeV.341123.PwPy8EG_CT10_AZNLOCTEQ6L1_ggH125_tautaulh.1lep1tau_raw.root",
    "VBFHtautau_1tau1lep": "mc15_13TeV.341156.PwPy8EG_CT10_AZNLOCTEQ6L1_VBFH125_tautaulh.1lep1tau_raw.root",
    "Wplusenu_1lep1tau": "mc15_13TeV.361100.PwPy8EG_AZNLOCTEQ6L1_Wplusenu.1lep1tau_raw.root",
    "Wplusmunu_1lep1tau": "mc15_13TeV.361101.PwPy8EG_AZNLOCTEQ6L1_Wplusmunu.1lep1tau_raw.root",
    "Wplustaunu_1lep1tau": "mc15_13TeV.361102.PwPy8EG_AZNLOCTEQ6L1_Wplustaunu.1lep1tau_raw.root",
    "Wminusenu_1lep1tau": "mc15_13TeV.361103.PwPy8EG_AZNLOCTEQ6L1_Wminusenu.1lep1tau_raw.root",
    "Wminusmunu_1lep1tau": "mc15_13TeV.361104.PwPy8EG_AZNLOCTEQ6L1_Wminusmunu.1lep1tau_raw.root",
    "Wminustaunu_1lep1tau": "mc15_13TeV.361105.PwPy8EG_AZNLOCTEQ6L1_Wminustaunu.1lep1tau_raw.root",
    "Zee_1lep1tau": "mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.1lep1tau_raw.root",
    "Zmumu_1lep1tau": "mc15_13TeV.361107.PwPy8EG_AZNLOCTEQ6L1_Zmumu.1lep1tau_raw.root",
    "Ztautau_1lep1tau": "mc15_13TeV.361108.PwPy8EG_AZNLOCTEQ6L1_Ztautau.1lep1tau_raw.root",
#    "ZqqZll": "mc15_13TeV.363356.Sh_221_NNPDF30NNLO_ZqqZll.1lep1tau_raw.root",
#    "WqqZll": "mc15_13TeV.363358.Sh_221_NNPDF30NNLO_WqqZll.1lep1tau_raw.root",
#    "WqqWlv": "mc15_13TeV.363359.Sh_221_NNPDF30NNLO_WpqqWmlv.1lep1tau_raw.root",
#    "WlvWqq": "mc15_13TeV.363360.Sh_221_NNPDF30NNLO_WplvWmqq.1lep1tau_raw.root",
#    "WlvZqq": "mc15_13TeV.363489.Sh_221_NNPDF30NNLO_WlvZqq.1lep1tau_raw.root",
#    "ZllZll": "mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.1lep1tau_raw.root",
#    "WlvZll": "mc15_13TeV.363491.Sh_221_NNPDF30NNLO_lllv.1lep1tau_raw.root",
#    "llvv": "mc15_13TeV.363492.Sh_221_NNPDF30NNLO_llvv.1lep1tau_raw.root",
#    "WlvZvv": "mc15_13TeV.363493.Sh_221_NNPDF30NNLO_lvvv.1lep1tau_raw.root",
    "ttbar_lep_1lep1tau": "mc15_13TeV.410000.PwPyEG_P2012_ttbar_hdamp172p5_nonallhad.1lep1tau_raw.root",
    "single_top_tchan_1lep1tau": "mc15_13TeV.410011.PwPyEG_P2012_singletop_tchan_lept_top.1lep1tau_raw.root",
    "single_antitop_tchan_1lep1tau": "mc15_13TeV.410012.PwPyEG_P2012_singletop_tchan_lept_antitop.1lep1tau_raw.root",
    "single_top_wtchan_1lep1tau": "mc15_13TeV.410013.PwPyEG_P2012_Wt_inclusive_top.1lep1tau_raw.root",
    "single_antitop_wtchan_1lep1tau": "mc15_13TeV.410014.PwPyEG_P2012_Wt_inclusive_antitop.1lep1tau_raw.root",
    "single_top_schan_1lep1tau": "mc15_13TeV.410025.PwPyEG_P2012_SingleTopSchan_noAllHad_top.1lep1tau_raw.root",
    "single_antitop_schan_1lep1tau": "mc15_13TeV.410026.PwPyEG_P2012_SingleTopSchan_noAllHad_antitop.1lep1tau_raw.root"
}
