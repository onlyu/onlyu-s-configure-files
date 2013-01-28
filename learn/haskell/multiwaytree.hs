
data Tree a = Node a [Tree a] deriving (Eq,Show)

nnodes :: Num b => Tree a -> b
nnodes (Node x ys) = 1 + foldr ((+) . nnodes ) 0 ys

treeToString :: Tree Char -> String
treeToString (Node x ys) = x : foldr ((++) . treeToString) "" ys ++ "^"

stringToTree' :: Monad m => String -> Tree Char -> m (String, Tree Char)
stringToTree' ('^':xs) t = return (xs, t)
stringToTree' (x:xs) (Node y ys) =
  do
    (rem@(rx:rxs), t) <- stringToTree' xs $ Node x []
    let newnode = Node y $ ys ++ [t]
    if rx == '^' then return (rem, newnode)
      else stringToTree' rem newnode
stringToTree' _ _ = fail "parse error"

stringToTree :: Monad m => String -> m (Tree Char)
stringToTree (x:xs) = do (_,t) <- stringToTree' xs $ Node x []
                         return t

ipl :: Tree a -> Int
ipl = helper 0
  where
    helper x (Node _ ys) = x + (sum . map (helper (x+1)) $ ys)

bottom_up :: Tree a -> [a]
bottom_up (Node x ys) = concatMap bottom_up ys ++ [x]

display_lisp :: Tree Char -> String
display_lisp (Node x []) = [x]
display_lisp (Node x ts) = '(' : x : concatMap ((:) ' ' . display_lisp) ts ++ ")"

tree1 = Node 'a' []

tree2 = Node 'a' [Node 'b' []]

tree3 = Node 'a' [Node 'b' [Node 'c' []]]
                          
tree4 = Node 'b' [Node 'd' [], Node 'e' []]

tree5 = Node 'a' [
                  Node 'f' [Node 'g' []],
                  Node 'c' [],
                  Node 'b' [Node 'd' [], Node 'e' []]
                 ]