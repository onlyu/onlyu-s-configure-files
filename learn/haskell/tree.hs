import Data.List
import Data.Maybe

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

hbalTree x = map fst . hbalTree'
    where hbalTree' 0 = [(Empty, 0)]
          hbalTree' 1 = [(Branch x Empty Empty, 1)]
          hbalTree' n =
            let t = hbalTree' (n-2) ++ hbalTree' (n-1)
            in  [(Branch x lb rb, h) | (lb,lh) <- t, (rb,rh) <- t
                                     , let h = 1 + max lh rh, h == n]
                                                    
hbalTreeNodes _ 0 = [Empty]
hbalTreeNodes x n = concatMap toFilteredTrees [minHeight .. maxHeight]
    where toFilteredTrees = filter ((n ==) . countNodes) . hbalTree x

          -- Similar to the Fibonacci sequence but adds 1 in each step.
          minNodesSeq = 0:1:zipWith ((+).(1+)) minNodesSeq (tail minNodesSeq)
          minNodes = (minNodesSeq !!)
          
          minHeight = ceiling $ logBase 2 $ fromIntegral (n+1)
          maxHeight = (fromJust $ findIndex (>n) minNodesSeq) - 1
          
countNodes Empty = 0
countNodes (Branch _ l r) = countNodes l + countNodes r + 1
