
% Brandon's consistency fail with ~a & a instead of $false 

fof(lt_irreflex, hypothesis,
    ![X] : (~lt(X, X))
).

fof(lt_assym, hypothesis,
    ![X, Y] : (lt(X, Y) <=> ~lt(Y, X))
).

fof(prove, conjecture, ~a & a).