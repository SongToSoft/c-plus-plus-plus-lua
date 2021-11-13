function fibbonacci(n)
    first = 0
    second = 1
    print("1       :   ", first)
    print("2       :   ", second)
    if (n < 3) then
        return
    end
    for i = 3, number, 1 do
        tmp = second
        second = first + second
        first = tmp
        print(i, ": ", second)
    end
end

function fibbonacciRecursive(first, second, count, number)
    print(count, ": ", first)
    if (count < number) then
        fibbonacciRecursive(second, first + second, count + 1, number)
    end
end

function main()
    print("Enter a count of fibbonacci:")
    number = io.read("*number")
    -- fibbonacci(number)
    fibbonacciRecursive(0, 1, 1, number)
end

main()
