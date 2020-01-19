module MyHomework where
    
    f :: Eq a => [a] -> [[a]]
    f [] = []
    f (x:xs) = [ [x] ++ has x xs ] ++ f (notHave x xs)
        where has x xs = [y | y <- xs, y == x]
              notHave x xs = [y | y <- xs, y /= x]
    -- =======================================================================================================

    group :: Eq a => [a] -> [[a]]
    group [] = []
    group [x] = [[x]]
    group (x:xs)
        | x == (head $ head $ group xs) = (x : (head (group xs))) : (tail (group xs))
        | otherwise = [x] : (group xs)

    -- ========================================================================================================
    
    qsort :: (a -> a -> Ordering) -> [a] -> [a]
    qsort _ [] = []
    qsort comparator (x:xs) = qsort comparator lower ++ [x] ++ qsort comparator upper
        where lower = [y | y <- xs, comparator y x /= GT]
              upper = [y | y <- xs, comparator y x == GT]

    -- ===================================================================================PENIS=================

    groupBy :: (a -> a -> Bool) -> [a] -> [[a]]
    groupBy _ [] = []
    groupBy _ [x] = [[x]]
    groupBy p (x:xs)
        | p x (head $ head $ groupBy p xs) = (x : (head (groupBy p xs))) : (tail (groupBy p xs))
        | otherwise = [x] : (groupBy p xs)
    -- =================================================HINTS====================================================
    on :: (b -> b -> c) -> (a -> b) -> a -> a -> c
    on op prev x y = prev x `op` prev y

    (&&&) :: (a -> b) -> (a -> c) -> a -> (b, c)
    (&&&) f g arg = (f arg, g arg)

    -- ===========================================================================================================

    sortOn :: Ord b => (a -> b) -> [a] -> [a]
    sortOn f = map snd . qsort (\(a,b)  (c,d) -> compare a c) . map (\x -> (f x, x))

    