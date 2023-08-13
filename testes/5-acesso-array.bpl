function f1 pi1 pa2
def
var vi1
vet va2 size ci10
reg ri3
enddef
vi1 = pi1 + ci1
set va2 index ci5 with ci2
set pa2 index ci0 with vi1
get va2 index ci8 to vi1
get pa2 index ci4 to ri3
return pi1
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
call f1 vi3 vr4 pa3
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