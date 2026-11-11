import cypari2
import time

def getPrimes(n):
    if n < 3:
        return []
    sito = [True] * n
    sito[0] = sito[1] = False
    i = 2
    while i * i < n:
        if sito[i]:
            for j in range(i * i, n, i):
                sito[j] = False
        i += 1
    return [k for k, jePrvocislo in enumerate(sito) if jePrvocislo]

def egcd(a, b):
    u0, u1, v0, v1 = 1, 0, 0, 1
    while b != 0:
        q, a, b = a // b, b, a % b
        u0, u1 = u1, u0 - q * u1
        v0, v1 = v1, v0 - q * v1
    return a, u0, v0

def modInverse(a, n):
    gcd, u, v = egcd(a, n)
    if gcd == 1:
        return u % n

def demo():
    # print(getPrimes(100000000))
    p = 8929
    q = 3533
    n = p * q
    fi_n = (p - 1) * (q - 1)
    #e = 65537
    e = 5
    d = modInverse(e, fi_n)

    x = 123
    y = pow(x, e, n)
    print(y)
    xx = pow(y, d, n)
    print(xx)

def rsa_decrypt(p, q, e, cipher):
    n = p * q
    fi_n = (p - 1) * (q - 1)
    d = modInverse(e, fi_n)

    return pow(cipher, d, n)

def factorize(n):
    if n % 2 == 0:
        return 2, n // 2
    i = 3
    while i * i <= n:
        if n % i == 0:
            return i, n // i
        i += 2
    raise ValueError(f"{n} je prvocislo")

def crack_message():
    p, q = factorize(43811) 
    data = open("crypto/RSA-1/sprava.enc", "rb").read()
    sprava = bytearray()
    for i in range(0, len(data), 2):
        c = int.from_bytes(data[i:i+2], "little")
        sprava.append(rsa_decrypt(p, q, 7, c))
    print(sprava.decode("iso-8859-2"))

def crack(n, y, pro = True):
    start = time.perf_counter()
    if pro:
        p, q = (int(x) for x in pari.factor(n)[0])
    else:
        p, q = factorize(n)
    duration = time.perf_counter() - start

    message = rsa_decrypt(p, q, 65537, y)
    print(f"p = {p}, q = {q}, message = {message}, duration = {duration:.4f}s")

if __name__ == "__main__":
    pari = cypari2.Pari()

    # sprava.enc (43811, 7)
    # crack_message()

    # task 1
    # crack(139357763092439, 43777336010553, False)
    # p = 9889037, q = 14092147, message = 111111111111, duration = 0.3029s

    # task 2
    # crack(26660317901772261199, 12575241328911977855, False)
    # p = 4375108721, q = 6093635519, message = 222222222222, duration = 260.3359s

    # task 3
    # crack(14095235505644163119978717, 1698807337024673366958273)
    # p = 3363995825503, q = 4190027644739, message = 333333333333, duration = 0.0107s

    # task 4
    # crack(2620872785180109408347655781687, 1363957964054024847622789702927)
    # p = 1481518863074599, q = 1769044492448113, message = 444444444444, duration = 0.0032s

    # task 5
    # crack(503904646087062569115617934030587309, 44576362600306107964786476479254021)
    # p = 657046748606874863, q = 766923582158321443, message = 555555555555, duration = 0.0200s

    # task 6
    # crack(209136298866545621207353511022259590742427, 196677266273980535928096179651672546166747)
    # p = 360195916292344957087, q = 580618184179536402821, message = 666666666666, duration = 0.0647s

    # task 7
    # crack(66604899422056757389496053619829175510145671847, 9165844488089234742422840339257953847347634062)
    # p = 226839685746209946305761, q = 293621017869751641608327, message = 777777777777, duration = 0.1232s

    # task 8
    # crack(14760905824960363007572237222303458594356027128046909, 8167237894173995987564647213999773859767421354037960)
    # p = 104650132339670262752754371, q = 141050044514514872528199679, message = 888888888888, duration = 0.3215s

    # task 9
    # crack(2535535456546525322526043826258946064934483954628894272177, 492602040213166561966870890113793329174098967674753758931)
    # p = 43360740989095509640892427551, q = 58475371931124734514897313327, message = 999999999999, duration = 1.0086s