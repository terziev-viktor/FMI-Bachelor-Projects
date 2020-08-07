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

% binary trees
ks(1, S, [S]).
ks(K, S, [H | T]) :-
    K > 1,
    K1 is K - 1,
    between(0, S, H),
    SH is S - H,
    ks(K1, SH, T).

nat(0).
nat(N) :- nat(K), N is K + 1.

% [] is non-valid empty tree or a tree with height of -1
% [Left, Root, Right] -> bin tree with root Root, left subtree Left and right subtree Right
% with height = max(height(Left), height(Right)) + 1
% [[], Value, []] -> Leaf (tree with height of 0)

% parametrization: Height, MaxVertex, VertexCount
bin_tree(Tree) :- 
    nat_triple(N, Max, K),
    Height is N - 1,
    bin_tree(Height, Max, K, Tree).
    
nat_triple(A, B, C) :-
    nat(N),
    ks(3, N, [A, B, C]).

% bin_tree(Height, Max, K, Tree) -> Tree is a binary tree with 
% K vertices, height of Height, a vertex with max
% value Max, and Max is a vertex of the tree Tree

max(S, B, B) :- S =< B.
max(B, S, B) :- S =< B.

bin_tree(Height, Max, K, Tree) :-
   Max1 is Max - 1,
   K1 is K - 1,
   choose_kn(K1, Max1, S),
   permutation([Max | S], V),
   bin_tree(Height, V, Tree). 

bin_tree(-1, _, []).
bin_tree(0, [X | _T], [[], X, []]).
bin_tree(0, [_X | T], [[], Root, []]) :-
    member(Root, T).

bin_tree(Height, V, [Left, Root, Right]) :- 
    Height > 0,
    subset(LeftV, V),
    subset(RightV, V),
    append(LeftV, [Root | RightV], PermutationOfV),
    permutation(PermutationOfV, V),
    H1 is Height - 1,
    between(-1, H1, LeftHeight),
    between(-1, H1, RightHeight),
    max(LeftHeight, RightHeight, M),
    Height is M + 1,
    bin_tree(LeftHeight, LeftV, Left),
    bin_tree(RightHeight, RightV, Right).

choose_kn(K, N, S) :- choose_from(K, 0, N, S).

choose_from(0, _Beg, _End, []).
choose_from(K, Beg, End, [H | T]) :-
    K > 0,
    K1 is K - 1,
    between(Beg, End, H),
    H1 is H + 1,
    choose_from(K1, H1, End, T).

ivo_bin_tree(Height, Max, K, Tree) :-
    Max1 is Max - 1,
    K1 is K - 1,
    choose_kn(K1, Max1, S),
    permutation([Max | S], V),
    ivo_bin_tree_v(Height, V, Tree).

ivo_bin_tree_v(Height, V, Tree) :-
    ivo_bin_tree_v_helper(Height, V, Tree, []).

ivo_bin_tree_v_helper(-1, V, [], V).
ivo_bin_tree_v_helper(Height, [Root | Vs], Tree, Rest) :-
    Height >= 0,
    H1 is Height - 1,
    between(-1, H1, LeftHeight),
    between(-1, H1, RightHeight),
    valid_heights(LeftHeight, RightHeight, H1),
    ivo_bin_tree_v_helper(LeftHeight, Vs, LeftTree, RightVertices),
    ivo_bin_tree_v_helper(RightHeight, RightVertices, RightTree, Rest),
    Tree = [LeftTree, Root, RightTree]. 

valid_heights(H, _, H).
valid_heights(_, H, H).

% Prime numbers
prime(P) :-
    nat(P),
    P > 1,
    is_prime(P).

is_prime(P) :-
    P > 1,
    P1 is P - 1,
    not((
        between(2, P1, D),
            P mod D =:= 0
        )).

primes(I, Ps) :-
    I1 is I - 1,
    range(2, I1, L),
    filter(L, Ps).

range(A, B, []) :- A > B.
range(A, B, [A | T]) :- A =< B, A1 is A + 1, range(A1, B, T).

filter([], []).
filter([N | Ns], T) :- not(condition(N)), filter(Ns, T).
filter([N | Ns], [N | T]) :- condition(N), filter(Ns, T).
condition(X) :- is_prime(X), X mod 6 =:= 1.

len([], 0).
len([_H | T], N) :- len(T, K), N is K + 1.

q(I, K, Ps) :- primes(I, Ps), len(Ps, K).

% ------- Homework -----------
check(X) :- nat(I), q(I, K, _), X is K + I.

special([ [_, _, _] ]).
special([ [_, _, _] | T]) :- special(T).

x_coherent(X, Y) :- 
    not(( 
        member(A, Y), 
        member(B, Y), 
        not( member([A, A, B], X)) 
       )),
    special(X).


split_x_coherent(X, Y, A, B) :- 
    append(A, B, Y),
    x_coherent(X, A),
    x_coherent(X, B).

% ------- Homework -----------

