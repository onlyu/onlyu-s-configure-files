
queens1 :: Int -> [[Int]]
queens1 n = map reverse $ queens' n
    where queens' 0       = [[]]
          queens' k       = [q:qs | q <- [1..n], qs <- queens' (k-1), isSafe q qs]
          isSafe   try qs = not (try `elem` qs || sameDiag try qs)
          sameDiag try qs = any (\(colDist, q) -> abs (try - q) == colDist) $ zip [1..] qs

queens2 :: Int -> [[Int]]
queens2 n = map reverse $ queens' n
    where queens' 0       = [[]]
          queens' k       = [q:qs | qs <- queens' (k-1), q <- [1..n], isSafe q qs]
          isSafe   try qs = not (try `elem` qs || sameDiag try qs)
          sameDiag try qs = any (\(colDist,q) -> abs (try - q) == colDist) $ zip [1..] qs
