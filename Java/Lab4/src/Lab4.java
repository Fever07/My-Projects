import java.lang.Comparable;

class Time implements Comparable<Time> {

    int hour;
    int minute;

    Time(int a, int b) {
        hour = a;
        minute = b;
    }

    public int compareTo(Time t) {
        if (hour != t.hour)
            return hour - t.hour;
        else
            return minute - t.minute;
    }

    public String toString() {
        return new String(Integer.toString(hour) + "h " + Integer.toString(minute) + "m");
    }

}


class Tree<T extends Comparable<T>, P> {

    class Node<E extends Comparable<E>, Q> {
        Q data;
        E key;
        Node<E, Q> left;
        Node<E, Q> right;

        Node(E k, Q d) {
            left = null;
            right = null;
            data = d;
            key = k;
        }

    }

    Node<T, P> top;

    Tree() {
        top = null;
    }

    private void insert(Node<T, P> p, T k, P x) {
        if (top == null) {
            top = new Node<T, P>(k, x);
        } else {
            if (k.compareTo(p.key) == 0) {
                p.data = x;
                return;
            }
            if (k.compareTo(p.key) < 0)
                if (p.left == null) {
                    p.left = new Node<T, P>(k, x);
                    return;
                } else
                    insert(p.left, k, x);
            if (k.compareTo(p.key) > 0)
                if (p.right == null) {
                    p.right = new Node<T, P>(k, x);
                    return;
                } else
                    insert(p.right, k, x);
        }
    }

    public void insert(T k, P x) {
        insert(top, k, x);
    }

    public void delete(T k) {
        if (top == null)
            return;
        Node<T, P> found = top;
        Node<T, P> par = null;
        while (found != null) {
            if (k.compareTo(found.key) == 0)
                break;
            else
                par = found;
            if (k.compareTo(found.key) < 0) {
                found = found.left;
            } else found = found.right;
        }
        if (found != null) {
            if (found.left == null) {
                if (found == top)
                    top = found.right;
                else if (found.key.compareTo(par.key) < 0)
                    par.left = found.right;
                else
                    par.right = found.right;
            } else if (found.right == null) {
                if (found == top)
                    top = found.left;
                else if (found.key.compareTo(par.key) < 0)
                    par.left = found.left;
                else
                    par.right = found.left;
                return;
            }else if (found.right.left == null) {
                if (found == top) {
                    top = found.right;
                    top.left = found.left;
                } else {
                    if (found.key.compareTo(par.key) < 0)
                        par.left = found.right;
                    else
                        par.right = found.right;
                    found.right.left = found.left;
                }
            } else {
                par = found.right;
                while (par.left.left != null)
                    par = par.left;
                found.data = par.left.data;
                found.key = par.left.key;
                par.left = par.left.right;
            }
        }
    }

    interface Method<E extends Comparable<E>, Q> {
        void method(Tree<E, Q>.Node<E, Q> arg);
    }

    public class Print implements Method<T, P> {
        public void method(Node<T, P> arg) {
            System.out.println(arg.key + " - " + arg.data + "; ");
        }
    }

    private void goLPR(Node<T, P> p, Method<T, P> m) {
        if (p == null)
            return;
        else {
            goLPR(p.left, m);
            m.method(p);
            goLPR(p.right, m);
        }
    }

    public void goLPR(Method<T, P> m) {
        goLPR(top, m);
    }

    private void goPLR(Node<T, P> p, Method<T, P> m) {
        if (p == null)
            return;
        else {
            m.method(p);
            goPLR(p.left, m);
            goPLR(p.right, m);
        }
    }

    public void goPLR(Method<T, P> m) {
        goPLR(top, m);
    }

    private void goLRP(Node<T, P> p, Method<T, P> m) {
        if (p == null)
            return;
        else {
            goLRP(p.left, m);
            goLRP(p.right, m);
            m.method(p);
        }
    }

    public void goLRP(Method<T, P> m) {
        goLRP(top, m);
    }

    private Node<T, P> find(Node<T, P> p, T k) {
        if (p == null)
            return null;
        else {
            if (k.compareTo(p.key) == 0)
                return p;
            if (k.compareTo(p.key) < 0)
                return find(p.left, k);
            if (k.compareTo(p.key) > 0)
                return find(p.right, k);
            return null;
        }
    }

    public P find(T k) {
        return find(top, k).data;
    }

}

public class Lab4 {
    public static void main(String[] args) {
        Tree tree = new Tree<Time, Integer>();
        tree.insert(new Time(2, 14), 7);
        tree.insert(new Time(2, 17), 2);
        tree.insert(new Time(1, 11), 10);
        tree.insert(new Time(3, 10), 5);
        tree.insert(new Time(3, 9), 5);
        tree.insert(new Time(2, 15), 8);
        tree.insert(new Time(0, 37), 4);
        tree.insert(new Time(1, 20), 13);
        System.out.println("Tree: ");
        tree.goLPR(tree.new Print());
        tree.delete(new Time(2, 17));
        tree.delete(new Time(1, 11));
        System.out.println("Tree: ");
        tree.goLPR(tree.new Print());
    }

}
