infix 1 ==>
(==>) :: Bool -> Bool -> Bool
True ==> x = x
False ==> _ = True

infix 1 <=>

(<=>) :: Bool -> Bool -> Bool
x <=> y = x == y

infixr 2 <+>
(<+>) :: Bool -> Bool -> Bool
x <+> y = x /= y

valid1 :: (Bool -> Bool) -> Bool
valid1 bf = and [ bf p | p <- [True, False]]

valid2 :: (Bool -> Bool -> Bool) -> Bool
valid2 bf = and [ bf p q | p <- [True, False],
                  q <- [True, False]]

valid3 :: (Bool -> Bool -> Bool -> Bool) -> Bool
valid3 bf = and [ bf p q r | p <- [True, False],
                  q <- [True, False],
                  r <- [True, False]]

valid4 :: (Bool -> Bool -> Bool -> Bool -> Bool) -> Bool
valid4 bf = and [ bf p q r s | p <- [True, False],
                  q <- [True, False],
                  r <- [True, False],
                  s <- [True, False]]

logEquiv1 :: (Bool -> Bool) -> (Bool -> Bool) -> Bool
logEquiv1 bf1 bf2 = and [ bf1 p <=> bf2 p | p <- [True, False]]

logEquiv2 :: (Bool -> Bool -> Bool) -> (Bool -> Bool -> Bool) -> Bool
logEquiv2 bf1 bf2 = and [ bf1 p q <=> bf2 p q | p <- [True, False],
                          q <- [True, False]]

logEquiv3 :: (Bool -> Bool -> Bool -> Bool) -> (Bool -> Bool -> Bool -> Bool) -> Bool
logEquiv3 bf1 bf2 = and [ bf1 p q r <=> bf2 p q r | p <- [True, False],
                          q <- [True, False],
                          r <- [True, False]]

-- theorem
doubleNegation p = p <=> not (not p)

idempotence1 p = p && p <=> p
idempotence2 p = p || p <=> p

formula3_1 p q = (p ==> q) <=> not p || q
formula3_2 p q = not (p ==> q) <=> p && not q

formula4_1 p q = (not p ==> not q) <=>  (q ==> p)
formula4_2 p q = (p ==> not q) <=> (q ==> not p)
formula4_3 p q = (not p ==> q) <=> (not q ==> p)

formula5_1 p q = (p <=> q) <=> ((p ==> q) && (q ==> p))
formula5_2 p q = (p <=> q) <=> ((p && q) || (not p && not q))

commutativity1 p q = p && q <=> q && p
commutativity2 p q = p || q <=> q || p

demorgan1 p q = not (p && q) <=> not p || not q
demorgan2 p q = not (p || q) <=> not p && not q

associativity1 p q r = p && (q && r) <=> (p && q) && r
associativity2 p q r = p || (q || r) <=> (p || q) || r

distribution1 p q r = p && (q || r) <=> (p && q) || (p && r)
distribution2 p q r = p || (q && r) <=> (p || q) && (p || r)

formula3 p q = p
formula4 p q = (p <+> q) <+> q

a = logEquiv2 formula3 formula4
