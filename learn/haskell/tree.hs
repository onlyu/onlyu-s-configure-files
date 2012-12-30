import Data.List

data Tree a = Empty | Branch a (Tree a) (Tree a)
                      deriving (Show, Eq)

-- cbalTree
cbalTree :: Int -> [Tree Char]
cbalTree 0 = [Empty]
cbalTree n = concat $ map incTree (cbalTree (n-1))

incTree :: Tree Char -> [Tree Char]
incTree Empty = [Branch 'x' Empty Empty]
incTree (Branch x left right)
  | l == r = map head . group $ incLeft ++ incRight
  | l < r = incLeft
  | otherwise = []
  where
    l = sizeTree left
    r = sizeTree right
    incLeft = map head . group $ map (\y -> Branch x y right) (incTree left)
    incRight = map head . group $ map (\y -> Branch x left y) (incTree right)

sizeTree :: Tree a -> Int
sizeTree Empty = 0
sizeTree (Branch x left right) = 1 + (sizeTree left) + (sizeTree right)

symmetric :: Tree a -> Bool
symmetric Empty = True
symmetric (Branch x l r) = mirror l r
  where
    mirror Empty Empty = True
    mirror (Branch _ xl xr) (Branch _ yl yr) = mirror xl yr && mirror xr yl
    mirror _ _ = False


add :: Ord a => a -> Tree a -> Tree a
add x Empty = Branch x Empty Empty
add x t@(Branch y l r) = case compare x y of
  LT -> Branch y (add x l) r
  GT -> Branch y l (add x r)
  EQ -> t

construct xs = foldl (flip add) Empty xs  


symCbalTrees n =
  if n `mod` 2 == 0 then [] else
    [Branch 'x' t (reverseTree t) | t <- cbalTree (n `div` 2)]

reverseTree Empty = Empty
reverseTree (Branch x l r) = Branch x (reverseTree r) (reverseTree l)

hbalTree :: a -> Int -> [Tree a]
hbalTree x n = filter ((== n) . deep) $ hbalTree' x (2 ^ n - 1)
hbalTree' _ 0 = [Empty]
hbalTree' x n = concat . map (add x) $  hbalTree' x (n-1)
  where
    add x Empty = [(Branch x Empty Empty)]
    add x (Branch y l r) 
      | deepl == deepr = addLeft ++ addRight
      | deepl < deepr = addLeft
      | otherwise = addRight
      where
        deepl = deep l
        deepr = deep r
        addLeft = map (\t -> Branch y t r) (add x l)
        addRight = map (\t -> Branch y l t) (add x r)

deep :: Tree a -> Int
deep Empty = 0
deep (Branch _ l r) = max (deep l + 1) (deep r + 1)
