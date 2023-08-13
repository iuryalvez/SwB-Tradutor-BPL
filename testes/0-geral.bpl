function f1 pi1 pa2 pa3
def
var vi1
vet va2 size ci4
reg vr3
var vi4
reg vr5
var vi6
reg vr7
vet va8 size ci8
vet va9 size ci1
vet va10 size ci7
reg vr11
var vi12
enddef
vi1 = ci10
vr3 = ci20
vi4 = ci30
vi6 = ci40
vr7 = ci50
set va2 index ci0 with pi1
set va2 index ci1 with vi1
set va2 index ci3 with ci60
get va10 index ci6 to vi4
get va10 index ci4 to vr11
get va10 index ci3 to vr12
get pa2 index ci100 to vr7
set pa3 index ci50 with vr7
if pi1 le vr7
set va2 index ci2 with vr3
get va10 index ci5 to vi6
return vi6
endif
if vi1 gt vr11
set pa3 index ci2 with vr3
get va10 index ci0 to vr5
return vr11
endif
if vi1 gt vr11
get pa3 index ci0 to vi12
set pa3 index ci40 with ci10
return ci10
endif
if vi1 eq vr7
vr7 = vr3 - pi1
vi3 = ci-200 * vi1
vr3 = vr3 * pi1
vr3 = ci-128 / ci3
vr11 = call f1 vi1 va8 va9
return vr11
endif
pi1 = vi6 + vr7
vr7 = vi6 + pi1
vi6 = pi1 + vr7
pi1 = vi6 - vr7
vr7 = vi6 - pi1
vi6 = pi1 - vr7
pi1 = vi6 / vr7
vr7 = vi6 / pi1
vi6 = pi1 / vr7
pi1 = vi6 * vr7
vr7 = vi6 * pi1
vi6 = pi1 * vr7
call f1 pi1 va10 va9
return ci10
end

function f2 pa1 pi2 pi3
def
vet va1 size ci2
vet va2 size ci5
var vi3
reg vr4
var vi5
reg vr6
enddef
call f1 vi3 pa1 va2
get pa1 index ci0 to pi2
get pa1 index ci10 to vi3
get pa1 index ci5  to vr4
get va1 index ci1 to pi2
get va2 index ci4 to vi3
get va1 index ci0  to vr4
set pa1 index ci0 with pi2
set pa1 index ci10 with vi3
set pa1 index ci5 with vr4
set va1 index ci1 with pi2
set va2 index ci4 with vi3
set va1 index ci0 with vr4
end

function f3 pi1 pi2 pi3
def
var vi1
var vi2
var vi3
var vi4
reg vr5
reg vr6
reg vr7
vet va8 size ci10
enddef
if vr7 eq vr7
vi3 = vi1
vi4 = vr2
endif
vi2 = pi3
vi1 = ci20
vi2 = ci-200 + vi4
vr2 = vr3 + pi1
vi2 = ci-200 - vi4
call f2 va8 pi1 vi2
if vr5 le vi1
vr2 = vr3 - pi1
vi2 = ci-200 * vi4
vr2 = vr3 * pi1
return pi1
endif
vr2 = ci-128 / ci3
vr2 = vr3 / pi3
vr2 = pi2 / vi3
return vr2
end

function f4 pi1
def
var vi1
vet va2 size ci10
var vi3
vet va4 size ci1
vet va5 size ci13
enddef
vi1 = ci3
vi3 = ci4
if vi1 ne vi3 
vi1 = ci0
endif
if vi1 lt vi3 
get va2 index ci8 to vi1
vi1 = call f1 va2 va4 va5
endif
if vi3 le vi1 
return vi1
endif
if pi1 le ci10 
return ci0
endif
if ci-10 gt ci10
get va2 index ci2 to vi3
endif
return ci-1
end

function f5 pi1 pa2
def
enddef
return pi1
end

function f6 pa1
def
reg vr1
var vi2
vet va3 size ci30
enddef
vr1 = ci1
vi2 = call f5 vr1 va3
vi2 = call f5 ci5 pa1
return vi2
end
