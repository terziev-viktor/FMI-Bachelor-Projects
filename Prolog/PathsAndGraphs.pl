path([_, _], S, S, [S]).
path([V, E], S, F, [S, X | P]) :-
    edge([S, X], E),
    path([V, E], X, F, [X | P]).

ivo_path([_, _], S, S, [S]).
ivo_path([V, E], S, F, P) :-
    subset(M, V),
    member(S, M),
    member(F, M),
    perm(P, M),
    is_legit_path(P, [V, E]).

is_legit_path([_X], [_V, _E]).
is_legit_path([S, F | T], [V, E]) :-
    edge([S, F], E),
    is_legit_path([F | T], [V, E]).

edge([A, B], E) :- member([A, B], E).
edge([A, B], E) :- member([B, A], E).

subset([], _).
subset([H | S], T) :- append(Beg, [H | End], T), append(Beg, End, NoH), subset(S, NoH).

sameset(A, B) :- subset(A, B), subset(B, A).

perm([], []).
perm(P, [H | T]) :- perm(Q, T), inserted(H, Q, P).

inserted(Element, A, B) :- append(Abegin,  AEnd, A), append(Abegin, [Element | AEnd], B).

in(X, [H | T]) :- X = H ; in(X, T).

% binary tries
get_KS(1, S, [S]).
gen_KS(K, S, [H | T]) :-
    K > 1,
    K1 is K - 1,
    between(0, S, H),
    SH is S - H,
    gen_KS(K1, SH, T).

nat(0).
nat(N) :- nat(K), N is K + 1.

