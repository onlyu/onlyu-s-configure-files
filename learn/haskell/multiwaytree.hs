
data Tree a = Node a [Tree a] deriving (Eq,Show)

nnodes :: Num b => Tree a -> b
nnodes (Node x ys) = 1 + foldr ((+) . nnodes ) 0 ys