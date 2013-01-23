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

-- Count the leaves of a binary tree
countLeaves :: Tree a -> Int
countLeaves Empty = 0
countLeaves (Branch _ Empty Empty) = 1
countLeaves (Branch _ l r) = countLeaves l + countLeaves r

leaves :: Tree a -> [a]
leaves Empty = []
leaves (Branch x Empty Empty) = [x]
leaves (Branch _ l r) = leaves l ++ leaves r

internals :: Tree a -> [a]
internals Empty = []
internals (Branch _ Empty Empty) = []
internals (Branch x l r) = x : internals l ++ internals r

atLevel :: Tree a -> Int -> [a]
atLevel Empty _ = []
atLevel (Branch x _ _) 1 = [x]
atLevel (Branch _ l r) level = atLevel l (level - 1) ++ atLevel r (level - 1)

completeBinaryTree :: Int -> Tree Char
completeBinaryTree 0 = Empty
completeBinaryTree 1 = Branch 'x' Empty Empty
completeBinaryTree n = Branch 'x' (completeBinaryTree x) (completeBinaryTree y)
  where
    h = ceiling $ logBase 2 $ fromIntegral (n+1)
    a = n - 2 ^ (h - 2)
    x = if a < 2^(h-1) then a else 2^(h-1)-1
    y = n - x - 1

isCompleteBinaryTree :: Tree a -> Bool
isCompleteBinaryTree Empty = True
isCompleteBinaryTree (Branch x l r) = 
  let
    ln = countNodes l
    rn = countNodes r
    lh = height l
    rh = height r
  in
   case lh - rh of
     0 -> ln == 2 ^ lh - 1
     1 -> rn == 2 ^ rh - 1
     _ -> False

height :: Tree a -> Int
height Empty = 0
height (Branch x l r) = 1 + (max (height l) (height r))

layout :: Tree a -> Tree (a, (Int, Int))
layout t = layout' t 0 1
  where
    layout' Empty _ _ = Empty
    layout' (Branch ax l r) x y = Branch (ax, (rootx, y)) (layout' l x nexty) (layout' r rootx nexty)
      where
        rootx = countNodes l + 1 + x
        nexty = y + 1


layout2 :: Tree a -> Tree (a, (Int, Int))
layout2 t = layout2' t 0 1 h
  where
    h = height t
    layout2' Empty _ _ _= Empty
    layout2' (Branch ax l r ) x y h = Branch (ax, (offset, y)) layoutL layoutR
      where
        layoutL = layout2' l x (y + 1) (h - 1)
        layoutR = layout2' r (offset + 2^(h-2)) (y+1) (h-1)
        offset = if x == 0 then offset' layoutL + 2^(h-2) else x
        offset' Empty = 0
        offset' (Branch (ax, (x,y)) _ _) = x

layout3 :: Tree a -> Tree (a, (Int, Int))
layout3 t = layout' x1 1 sep1 t
  where d = depth t
        dl = depthLeft t
        sep1 = 2 ^ (d-2)
        x1 = 2^(d-1) - 2^(d-dl) + 1
        layout' _ _ _ Empty = Empty
        layout' x y sep (Branch ax l r) =
          Branch (ax, (x,y))
          	  (layout' (x-sep) (y+1) (sep `div` 2) l)
          	  (layout' (x+sep) (y+1) (sep `div` 2) r)

depth :: Tree a -> Int
depth Empty = 0
depth (Branch _ l r) = 1 + max (depth l) (depth r)

depthLeft :: Tree a -> Int
depthLeft Empty = 0
depthLeft (Branch _ l _) = 1 + depthLeft l

stringToTree :: String -> Tree Char
stringToTree s = fst $ help s
  where
    help (')':',':xs) = (Empty, xs)
    help (')':xs) = help xs
    help (',':xs) = (Empty, xs)
    help [] = (Empty, [])
    help (x:'(':xs) = (Branch x l r, remainr)
      where (l, remainl) = help xs
            (r, remainr) = help remainl

treeToString :: Tree Char -> String
treeToString Empty = []
treeToString (Branch c l r) = c:'(': treeToString l ++ ',':treeToString r ++ ")"

treeToPreorder :: Tree Char -> String
treeToPreorder Empty = ""
treeToPreorder (Branch x l r) = x : treeToPreorder l ++ treeToPreorder r

treeToInorder :: Tree Char -> String
treeToInorder Empty = ""
treeToInorder (Branch x l r) = treeToInorder l ++ x : treeToInorder r 

preInOrder :: String -> String -> Tree Char
preInOrder [] _ = Empty
preInOrder (x:xs) ys = Branch x (preInOrder lpre lin) (preInOrder rpre rin)
  where
    (lin,_:rin) = span (/= x) ys
    (lpre,rpre) = splitAt (length lin) xs

tree2ds :: Tree Char -> String
tree2ds Empty = "."
tree2ds (Branch x l r) = x : tree2ds l ++ tree2ds r

ds2tree' :: Monad m => String -> m (String, Tree Char)
ds2tree' "." = return ("", Empty)
ds2tree' (x:xs) = do (remain_, l) <- ds2tree' xs
                     (remain, r) <- ds2tree' remain_
                     return (remain, Branch x l r)
ds2tree' _ =  fail "syntax error"

ds2tree :: Monad m => String -> m (Tree Char)
ds2tree str = do (_,t) <- ds2tree' str
                 return t

-- test data
tree65 = Branch 'n'
         (Branch 'k'
          (Branch 'c'
           (Branch 'a' Empty Empty)
           (Branch 'e'
            (Branch 'd' Empty Empty)
            (Branch 'g' Empty Empty)
           )
          )
          (Branch 'm' Empty Empty)
         )
         (Branch 'u'
          (Branch 'p'
           Empty
           (Branch 'q' Empty Empty)
          )
          Empty
         )