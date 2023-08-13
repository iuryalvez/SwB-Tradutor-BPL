function f1 pa1
def
var vi1
reg vr2 
vet va3 size ci6
var vi4
vet va5 size ci7
var vi6
reg vr7
var vi8
vet va9 size ci8
reg vr10
vet va11 size ci3
reg vr12
enddef
vi1 = vr7
vr7 = vi6
vi6 = vi1
vr12 = vr10
vr10 = vi8 + vr12
vi1 = vr7 - vi4
vr7 = vi6 / vr10
vi6 = vi1 / vr12
vr12 = vr10 * vi8
vr10 = vi8 * vr10
return vr12
end

function f2 pi1 pi2 pi3
def
vet va1 size ci1
vet va2 size ci2
vet va3 size ci3
vet va4 size ci4
enddef
return pi3
end

function f3 pi1 pa2 pi3
def
reg vr1
reg vr2
reg vr3
reg vr4
enddef
return vr2
end

function f4 pi1 pi2 pa3
def
reg vr1
var vi2
reg vr3 
vet va4 size ci4
var vi5
var vi6
reg vr7
var vi8
vet va9 size ci4
vet va10 size ci4
reg vr11
vet va12 size ci4
enddef
return vi6
end

function f5 pa1 pi2 pi3
def
var vi1
var vi2
var vi3
var vi4
vet va5 size ci2
vet va6 size ci3
vet va7 size ci4
vet va8 size ci5
reg vr9 
reg vr10
reg vr11
reg vr12
enddef
return vi3
end

function f6 pi1
def
var vi1
reg vr2
enddef
vi1 = ci1
vr2 = vi1
vi1 = pi1 + vr2
vr2 = vi1 * ci-5
return vi1
end

function f7 pi1 pi2 pi3
def
var vi1
var vi2
var vi3
var vi4
reg vr2
reg vr3
reg vr4
enddef
vi3 = vi1
vi4 = vr2
vi2 = pi3
vi1 = ci20
vi2 = ci-200 + vi4
vr2 = vr3 + pi1
vi2 = ci-200 - vi4
vr2 = vr3 - pi1
vi2 = ci-200 * vi4
vr2 = vr3 * pi1
vr2 = ci-128 / ci3
vr2 = vr3 / pi3
vr2 = pi2 / vi3
return vr2
end
