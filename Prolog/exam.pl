% exam problem 1
singleton([_]).

appended([], L, L).
appended([H | L], T, [H | R]) :- appended(L, T, R).

splitted(L, Left, Right) :- 
    appended(Left, Right, L),
    Left \= [],
    Right \= [].

expr([X], num(X)).
expr(L, sub(Left, Right)) :- 
    not(singleton(L)),
    splitted(L, Begin, End),
    expr(Begin, Left),
    expr(End, Right).

expr(L, plus(Left, Right)) :- 
    not(singleton(L)),
    splitted(L, Begin, End),
    expr(Begin, Left),
    expr(End, Right).

eval_expr(num(X), X).

eval_expr(sub(X, Y), R) :-
    eval_expr(X, R1),
    eval_expr(Y, R2),
    R is R1 - R2.

eval_expr(plus(X, Y), R) :-
    eval_expr(X, R1),
    eval_expr(Y, R2),
    R is R1 + R2.

eval_to_z(L, Z) :- expr(L, Expr), eval_expr(Expr, Z).

% exam problem 2
max(L, M) :- member(M, L), inner_max(L, M).
inner_max([A], B) :- B >= A.
inner_max([H | T], M) :- M >= H, inner_max(T, M).

vectors(0, _, [], []).
vectors(N, K, [A | As], [B | Bs]) :-
    N > 0,
    MinusK is -K,
    between(MinusK, K, A),
    between(MinusK, K, B),
    N1 is N - 1,
    vectors(N1, K, As, Bs).

dot_product([A], [B], V) :- V is A * B.
dot_product([A | As], [B | Bs], S) :- AB is A * B, dot_product(As, Bs, K), S is AB + K.

pred(A, B) :- dot_product(A, B, 0).

module_product([B], B) :- B > 0.
module_product([B], K) :- B < 0, K is -B.
module_product([B | Bs], P) :- B > 0, module_product(Bs, P1), P is B * P1.
module_product([B | Bs], P) :- B < 0, module_product(Bs, P1), MinusB is -B, P is MinusB * P1.

candidates(N, K, A, B, V) :- 
    vectors(N, K, A, B),
    pred(A, B),
    module_product(B, BProduct),
    dot_product(A, A, AProduct),
    V is BProduct * AProduct.

max_candidates(N, K, A, B) :-
    candidates(N, K, A, B, V),
    not((
        candidates(N, K, _, _, U),
        U > V
        )).
