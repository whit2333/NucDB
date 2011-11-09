import ROOT
ROOT.gROOT.SetStyle("Plain")
from array import array
from ROOT import TCanvas

c = TCanvas("asdf","asdf")
c.Divide(3,3)
c.cd(1)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76, 1.84, 1.92, 2.0])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [0.309, -0.273, -0.406, -0.223, -0.124, -0.077, -0.036, 0.14, 0.063, 0.055, -0.254, -0.084])
yerrminus = array("f", [0.5689894550868233, 0.21676023620581336, 0.18740864441108365, 0.20308126452235814, 0.16341664541900253, 0.1320984481362291, 0.11994165248152952, 0.10456098698845569, 0.10159724405711014, 0.08766413177577247, 0.08475848040166836, 0.07256031973468695])
yerrplus = array("f", [0.5689894550868233, 0.21676023620581336, 0.18740864441108365, 0.20308126452235814, 0.16341664541900253, 0.1320984481362291, 0.11994165248152952, 0.10456098698845569, 0.10159724405711014, 0.08766413177577247, 0.08475848040166836, 0.07256031973468695])
numpoints = 12
p1323_d1x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d1x1y1.SetName("/HepData/1323/d1x1y1")
p1323_d1x1y1.SetTitle("/HepData/1323/d1x1y1")
p1323_d1x1y1.Draw("AP")

c.cd(2)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76, 1.84, 1.92])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.327, -0.411, -0.316, -0.07, 0.086, 0.144, 0.147, 0.061, 0.0060, 0.024, -0.045])
yerrminus = array("f", [0.3282834141408914, 0.13580132547217644, 0.10872442227944924, 0.11851160280748886, 0.08780091115700338, 0.07244998274671982, 0.0674166151627327, 0.05604462507680821, 0.05412947441089743, 0.05166236541235796, 0.04876474136094643])
yerrplus = array("f", [0.3282834141408914, 0.13580132547217644, 0.10872442227944924, 0.11851160280748886, 0.08780091115700338, 0.07244998274671982, 0.0674166151627327, 0.05604462507680821, 0.05412947441089743, 0.05166236541235796, 0.04876474136094643])
numpoints = 11
p1323_d2x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d2x1y1.SetName("/HepData/1323/d2x1y1")
p1323_d2x1y1.SetTitle("/HepData/1323/d2x1y1")
p1323_d2x1y1.Draw("AP")

c.cd(3)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.529, -0.299, -0.106, -0.0050, 0.139, 0.34, 0.307, 0.195, 0.184])
yerrminus = array("f", [0.25564428411368795, 0.10791200118615168, 0.08668333173107735, 0.10196568050084302, 0.07983107164506813, 0.07559100475585703, 0.0718679344353238, 0.06037383539249432, 0.065])
yerrplus = array("f", [0.25564428411368795, 0.10791200118615168, 0.08668333173107735, 0.10196568050084302, 0.07983107164506813, 0.07559100475585703, 0.0718679344353238, 0.06037383539249432, 0.065])
numpoints = 9
p1323_d3x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d3x1y1.SetName("/HepData/1323/d3x1y1")
p1323_d3x1y1.SetTitle("/HepData/1323/d3x1y1")
p1323_d3x1y1.Draw("AP")

c.cd(4)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76, 1.84, 1.92, 2.0])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [0.033, -0.115, -0.172, -0.08, -0.059, -0.04, -0.022, 0.112, 0.058, 0.048, -0.202, -0.07])
yerrminus = array("f", [0.07161005515987263, 0.08509406559801923, 0.0810246875958186, 0.07051950084905594, 0.06865857557508749, 0.07863841300535, 0.0741484996476665, 0.07665507158694719, 0.0758023746329889, 0.06670832032063168, 0.07169379331573969, 0.0670820393249937])
yerrplus = array("f", [0.07161005515987263, 0.08509406559801923, 0.0810246875958186, 0.07051950084905594, 0.06865857557508749, 0.07863841300535, 0.0741484996476665, 0.07665507158694719, 0.0758023746329889, 0.06670832032063168, 0.07169379331573969, 0.0670820393249937])
numpoints = 12
p1323_d4x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d4x1y1.SetName("/HepData/1323/d4x1y1")
p1323_d4x1y1.SetTitle("/HepData/1323/d4x1y1")
p1323_d4x1y1.Draw("AP")

c.cd(5)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76, 1.84, 1.92])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.025, -0.088, -0.083, -0.0080, 0.024, 0.075, 0.08, 0.046, 0.016, 0.028, -0.016])
yerrminus = array("f", [0.018439088914585774, 0.03023243291566195, 0.02640075756488817, 0.026925824035672518, 0.025, 0.031384709652950434, 0.03130495168499706, 0.032310988842807024, 0.03289376840679705, 0.03269556544854363, 0.03361547262794322])
yerrplus = array("f", [0.018439088914585774, 0.03023243291566195, 0.02640075756488817, 0.026925824035672518, 0.025, 0.031384709652950434, 0.03130495168499706, 0.032310988842807024, 0.03289376840679705, 0.03269556544854363, 0.03361547262794322])
numpoints = 11
p1323_d5x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d5x1y1.SetName("/HepData/1323/d5x1y1")
p1323_d5x1y1.SetTitle("/HepData/1323/d5x1y1")
p1323_d5x1y1.Draw("AP")

c.cd(6)
xval = array("f", [1.12, 1.2, 1.28, 1.36, 1.44, 1.52, 1.6, 1.68, 1.76])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.022, -0.029, -0.01, 0.0030, 0.024, 0.086, 0.089, 0.072, 0.082])
yerrminus = array("f", [0.008944271909999158, 0.011401754250991379, 0.011704699910719625, 0.013038404810405297, 0.01392838827718412, 0.0194164878389476, 0.0206155281280883, 0.022203603311174516, 0.027018512172212593])
yerrplus = array("f", [0.008944271909999158, 0.011401754250991379, 0.011704699910719625, 0.013038404810405297, 0.01392838827718412, 0.0194164878389476, 0.0206155281280883, 0.022203603311174516, 0.027018512172212593])
numpoints = 9
p1323_d6x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d6x1y1.SetName("/HepData/1323/d6x1y1")
p1323_d6x1y1.SetTitle("/HepData/1323/d6x1y1")
p1323_d6x1y1.Draw("AP")

c.cd(7)
xval = array("f", [0.34, 0.53, 0.79, 1.1])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.027, -0.0080, 0.0080, 0.0070])
yerrminus = array("f", [0.013000000000000001, 0.004472135954999579, 0.004242640687119285, 0.0036055512754639895])
yerrplus = array("f", [0.013000000000000001, 0.004472135954999579, 0.004242640687119285, 0.0036055512754639895])
numpoints = 4
p1323_d7x1y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d7x1y1.SetName("/HepData/1323/d7x1y1")
p1323_d7x1y1.SetTitle("/HepData/1323/d7x1y1")
p1323_d7x1y1.Draw("AP")

c.cd(8)
xval = array("f", [0.34, 0.53, 0.79, 1.1])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.034, -0.013, 0.0090, 0.016])
yerrminus = array("f", [0.0080, 0.0070, 0.0080, 0.0090])
yerrplus = array("f", [0.0080, 0.0070, 0.0080, 0.0090])
numpoints = 4
p1323_d7x1y2 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d7x1y2.SetName("/HepData/1323/d7x1y2")
p1323_d7x1y2.SetTitle("/HepData/1323/d7x1y2")
p1323_d7x1y2.Draw("AP")

c.cd(9)
xval = array("f", [2.0, 2.0, 1.96, 1.8])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.027, -0.0080, 0.0080, 0.0070])
yerrminus = array("f", [0.013000000000000001, 0.004472135954999579, 0.004242640687119285, 0.0036055512754639895])
yerrplus = array("f", [0.013000000000000001, 0.004472135954999579, 0.004242640687119285, 0.0036055512754639895])
numpoints = 4
p1323_d7x2y1 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d7x2y1.SetName("/HepData/1323/d7x2y1")
p1323_d7x2y1.SetTitle("/HepData/1323/d7x2y1")
p1323_d7x2y1.Draw("AP")

xval = array("f", [2.0, 2.0, 1.96, 1.8])
xerrminus = array("f", [0.0, 0.0, 0.0, 0.0])
xerrplus = array("f", [0.0, 0.0, 0.0, 0.0])
yval = array("f", [-0.034, -0.013, 0.0090, 0.016])
yerrminus = array("f", [0.0080, 0.0070, 0.0080, 0.0090])
yerrplus = array("f", [0.0080, 0.0070, 0.0080, 0.0090])
numpoints = 4
p1323_d7x2y2 = ROOT.TGraphAsymmErrors(numpoints, xval, yval, xerrminus, xerrplus, yerrminus, yerrplus)
p1323_d7x2y2.SetName("/HepData/1323/d7x2y2")
p1323_d7x2y2.SetTitle("/HepData/1323/d7x2y2")
p1323_d7x2y2.Draw("AP")
