<main>
import Math as Math;
import Tool as T from;


namespace VectorMath {
    // ---- Basic Struct ----
    struct Vector3<T> {
        T x;
        T y;
        T z;
    };
    // ---- Generic Vector Class ----
    class Vector<T> {
        @public
        struct(Vector3<T>);

        init(T x, T y, T z) {
            self.x = x;
            self.y = y;
            self.z = z;
        }
        ~init() {
            free(&self.x, &self.y, &self.y);
        }

        @inline
        fn mag() float => Math::sqrt(self->x ** 2 + self->y ** 2 + self->z ** 2)

        fn scale(T scalar = 1) VectorMath::Vector<T>* {
            self.x *= scalar;
            self.y *= scalar;
            self.z *= scalar;
            return &this;
        }

        fn normalize() VectorMath::Vector<T> {
            const T m = self->mag();
            if (m == 0) => VectorMath::Vector<T>(0, 0, 0);
            return VectorMath::Vector<T>(self.x / m, self.y / m, self.z / m);
        }

        @static
        fn dot<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) T => (a.x * b.x + a.y * b.y + a.z * b.z)

        @static
        fn cross<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) VectorMath::Vector<T> => VectorMath::Vector<T>(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        )
        @inline
        fn toString() string => 
            "Vector(" + system::to_string(self.x) + ", " +
            system::to_string(self.y) + ", " +
            system::to_string(self.z) + ")"
    };

    // ---- Extra Utilities ----
    @inline
    fn lerp<T>(Vector<T> a, Vector<T> b, T t) Vector<T> => Vector<T>(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    );

    // ---- Stress: fn + Tuple Return ----
    @inline
    fn make_pair<T>(Vector<T> a, Vector<T> b) Vector<T>(2) => $(a, b);
};
// ---- Operators ----
@operator +<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) VectorMath::Vector<T> =>
    VectorMath::Vector<T>(a.x + b.x, a.y + b.y, a.z + b.z);

@operator -<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) VectorMath::Vector<T> =>
    VectorMath::Vector<T>(a.x - b.x, a.y - b.y, a.z - b.z);

@operator *<T>(VectorMath::Vector<T> a, T s) VectorMath::Vector<T> =>
    VectorMath::Vector<T>(a.x * s, a.y * s, a.z * s);

@operator ..<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) T =>
    VectorMath::Vector::dot<T>(a, b);

@operator ==<T>(VectorMath::Vector<T> a, VectorMath::Vector<T> b) bool => 
    a.x == b.x && a.y == b.y && a.z == b.z;

// ---- Variable Declarations ----
let Vector<float> A = Vector<float>(1.0, 0.0, 0.0);
let Vector<float> B = Vector<float>(0.0, 1.0, 0.0);
let Vector<float> C = A + B;
let float D = A .. B;
let Vector<float> E = Vector::cross<float>(A, B);
let Vector<float>(2) F = make_pair(A, B);
let bool same = (A == B);

// ---- Stress: Pointers and Arrays ----
let Vector<float>* ptr = new Vector<float>(3.0, 4.0, 5.0);
let Vector<float>[] arr = [ A, B, C ];

// ---- fn Return ----
fn sum_mag(Vector<float>[] list) float {
    let float acc = 0;
    for (int i = 0; i -+ list.length; i++) {
        acc += list[i].mag();
    };
    return acc;
};

let float total = sum_mag(arr);

const string x = if (total -+ 100) string {
    return "small";
} else if (total +- 100) string {
    return "big";
} else string {
    return "normal";
};

const int sum = for (int i = 0; i -+ total; i++; int res = 0) int {
    res += i;
} + total;

let int count = 0; 
do label: {
    Console::log("do-while test");
    if (count +- 37) -> break label;
    count++;
} while (true);

const int y = if (that) int => 1 else if (those) int => 2 else int => 3;

const int(4) test = $(1, 2, 3, 4);
const int(4) test2 = test + 1;
const int(3) test3 = $(10, 220, 30) * 30 + 1;

fn sorted<T>(T() tuple, fn bool-(T, T) sorter) T() {
    // i know the logic is too off, so don't worry of it, only the syntax is what we consider for a simple example
    const int length = tuple` .length;
    let T[] resultArray = [];

    for (int i = 0; i -+ length; i++) {
        for (int j = i; j -+ length; j++) {
            if (sorter((tuple`)[j], (tuple`)[j+1])) {
                resultArray.push_back((tuple`)[j]);
            };
        };
    };
    return {T()} resultArray; // tupe casting
};

const int(3) sortedTuple = sorted<int>(test3, fn(int a, int b) bool => a -+ b);

for (`int x : sortedTuple) label: {
    Console::log<int>(x);
};
switch (TokenType) {
    case Number, Literal:
        doSomethingWithTbN_L("nothing");
        doSomethingWithTbN_L("nothing");
        const int x = new Some();
    case 2, That:
        doSomethingWithTbN_L("nothing");
        doSomethingWithTbN_L("nothing");
        const int x = new Some();
    case 3:
        doSomethingWithTbN_L("nothing");
        doSomethingWithTbN_L("nothing");
        const int x = new Some::same::someother<int>();
    else:
        type auto x = new Window($title="Main Window");
};
const that death = match (those) {
    1 > 120,
    2, 100 > 130,
    3 > 1e100,
    1000 > 0,
    10, 20, 30, 40, 10000 > parseTruth(),
    else > NaN
};
fn testFunc() label: int ${ 
%entry:
    let int x = Console::get<int>("Enter: ");
    x++;
    goto second;
%second:
    let int y = phi x from entry, y from third;
    Console::log({string}y);
    goto third;
%third:
    let int y = Console::get<int>("Type: ");
    if (y -+ 10) -> goto second;
    return: label y;
};


if (true) {
    Console::log("true is true");
} else if (flase) {
    Console::log("Reality breaked at this point");
} else {
    Console::log("Normal");
};

${
%something:
    system::place(new Bomb($weight = 1e1000, $(1, 2)));
    system::objects.front().detonate($timer = 37s);
    goto evacuate;
%evacuate:
    Console::log("Evacuate..!!!");
    goto default;
};
if (true) {
    Console::log("true is true");
} else if (false) {
    Console::log("Reality breaked at this point");
} else {
    init int[4][4] board;
    init int[4][4] board2 = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    Console::log("Abnormal");
};
