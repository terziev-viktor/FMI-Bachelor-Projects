module MyHomework where
    group :: Eq a => [a] -> [[a]]
    group [] = []
    group (x:xs) = [ [x] ++ has x xs ] ++ group (notHave x xs)
        where has x xs = [y | y <- xs, y == x]
              notHave x xs = [y | y <- xs, y /= x]
    