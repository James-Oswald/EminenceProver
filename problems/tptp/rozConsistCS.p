
fof(lt_irreflex, hypothesis,
    ![X] : (~lt(X, X))
).

fof(lt_assym, hypothesis,
    ![X, Y] : ~(X=Y) => (lt(X, Y) <=> ~lt(Y, X))
).

fof(prove, conjecture, $false).