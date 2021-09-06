create view PARENTS as	
SELECT distinct P
FROM BST where P IS NOT NULL;