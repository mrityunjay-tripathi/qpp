/*
 * Quantum++
 *
 * Copyright (c) 2013 - 2016 Vlad Gheorghiu (vgheorgh@gmail.com)
 *
 * This file is part of Quantum++.
 *
 * Quantum++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Quantum++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quantum++.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
* \file number_theory.h
* \brief Number theory functions
*/

#ifndef NUMBER_THEORY_H_
#define NUMBER_THEORY_H_

// number theory functions

namespace qpp
{
/**
* \brief Simple continued fraction expansion
* \see qpp::contfrac2x()
*
* \param x Real number
* \param n Maximum number of terms in the expansion
* \param cut Stop the expansion when the next term is greater than \a cut
* \return Integer vector containing the simple continued fraction expansion
* of \a x. If there are \a m less than \a n terms in the expansion,
* a shorter vector with \a m components is returned.
*/
inline std::vector<int> x2contfrac(double x, idx n, idx cut = 1e5)
{
    // EXCEPTION CHECKS

    if (n == 0)
        throw Exception("qpp::x2contfrac()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    std::vector<int> result;

    for (idx i = 0; i < n; ++i)
    {
        if (x > 0)
        {
            result.push_back(std::llround(std::floor(x)));
            x = 1 / (x - std::floor(x));
        } else // x < 0
        {
            result.push_back(std::llround(std::ceil(x)));
            x = 1 / (x - std::ceil(x));
        }
        if (!std::isfinite(x) || std::abs(x) > cut)
            return result;
    }

    return result;
}

/**
* \brief Real representation of a simple continued fraction
* \see qpp::x2contfrac()
*
* \param cf Integer vector containing the simple continued fraction expansion
* \param n Number of terms considered in the continued fraction expansion.
* If \a n is greater than the size of \a cf,then all terms in \a cf
* are considered.
* \return Real representation of the simple continued fraction
*/
inline double contfrac2x(const std::vector<int>& cf, idx n)
{
    // EXCEPTION CHECKS

    if (cf.size() == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::ZERO_SIZE);

    if (n == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    if (n > cf.size())
        n = cf.size();

    if (n == 1) // degenerate case, integer
        return cf[0];

    double tmp = 1. / cf[n - 1];
    for (idx i = n - 2; i != 0; --i)
    {
        tmp = 1. / (tmp + cf[i]);
    }

    return cf[0] + tmp;
}

/**
* \brief Real representation of a simple continued fraction
* \see qpp::x2contfrac()
*
* \param cf Integer vector containing the simple continued fraction expansion
* \return Real representation of the simple continued fraction
*/
inline double contfrac2x(const std::vector<int>& cf)
{
    // EXCEPTION CHECKS

    if (cf.size() == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::ZERO_SIZE);
    // END EXCEPTION CHECKS

    if (cf.size() == 1) // degenerate case, integer
        return cf[0];

    double tmp = 1. / cf[cf.size() - 1];
    for (idx i = cf.size() - 2; i != 0; --i)
    {
        tmp = 1. / (tmp + cf[i]);
    }

    return cf[0] + tmp;
}

/**
* \brief Greatest common divisor of two integers
* \see qpp::lcm()
*
* \param m Integer
* \param n Integer
* \return Greatest common divisor of \a m and \a n
*/
inline bigint gcd(bigint m, bigint n)
{
    // EXCEPTION CHECKS

    if (m == 0 && n == 0)
        throw Exception("qpp::gcd()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    if (m == 0 || n == 0)
        return (std::max(std::abs(m), std::abs(n)));

    bigint result = 1;
    while (n)
    {
        result = n;
        n = m % result;
        m = result;
    }

    return (result > 0) ? result : -result;
}

/**
* \brief Greatest common divisor of a list of integers
* \see qpp::lcm()
*
* \param ns List of integers
* \return Greatest common divisor of all numbers in \a ns
*/
inline bigint gcd(const std::vector<bigint>& ns)
{
    // EXCEPTION CHECKS

    if (ns.size() == 0)
        throw Exception("qpp::gcd()", Exception::Type::ZERO_SIZE);
    // END EXCEPTION CHECKS

    bigint result = ns[0]; // convention: gcd({n}) = n
    for (idx i = 1; i < ns.size(); ++i)
    {
        result = gcd(result, ns[i]);
    }

    return (result > 0) ? result : -result;
}

/**
* \brief Least common multiple of two integers
* \see qpp::gcd()
*
* \param m Integer
* \param n Integer
* \return Least common multiple of \a m and \a n
*/
inline bigint lcm(bigint m, bigint n)
{
    if (m == 0 && n == 0)
        throw Exception("qpp::lcm()", Exception::Type::OUT_OF_RANGE);

    bigint result = m * n / gcd(m, n);

    return (result > 0) ? result : -result;
}

/**
* \brief Least common multiple of a list of integers
* \see qpp::gcd()
*
* \param ns List of integers
* \return Least common multiple of all numbers in \a ns
*/
inline bigint lcm(const std::vector<bigint>& ns)
{
    // EXCEPTION CHECKS

    if (ns.size() == 0)
        throw Exception("qpp::lcm()", Exception::Type::ZERO_SIZE);

    if (ns.size() == 1) // convention: lcm({n}) = n
        return ns[0];

    if (std::find(std::begin(ns), std::end(ns), 0) != std::end(ns))
        throw Exception("qpp::lcm()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    bigint result = ns[0]; // convention: lcm({n}) = n

    for (idx i = 1; i < ns.size(); ++i)
    {
        result = lcm(result, ns[i]);
    }

    return (result > 0) ? result : -result;
}

/**
* \brief Inverse permutation
*
* \param perm Permutation
* \return Inverse of the permutation \a perm
*/
inline std::vector<idx> invperm(const std::vector<idx>& perm)
{
    // EXCEPTION CHECKS

    if (!internal::_check_perm(perm))
        throw Exception("qpp::invperm()", Exception::Type::PERM_INVALID);
    // END EXCEPTION CHECKS

    // construct the inverse
    std::vector<idx> result(perm.size());
    for (idx i = 0; i < perm.size(); ++i)
        result[perm[i]] = i;

    return result;
}

/**
* \brief Compose permutations
*
* \param perm Permutation
* \param sigma Permutation
* \return Composition of the permutations \a perm \f$\circ\f$ \a sigma
*  = perm(sigma)
*/
inline std::vector<idx> compperm(const std::vector<idx>& perm,
                                 const std::vector<idx>& sigma)
{
    // EXCEPTION CHECKS

    if (!internal::_check_perm(perm))
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);
    if (!internal::_check_perm(sigma))
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);
    if (perm.size() != sigma.size())
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);
    // END EXCEPTION CHECKS

    // construct the composition perm(sigma)
    std::vector<idx> result(perm.size());
    for (idx i = 0; i < perm.size(); ++i)
        result[i] = perm[sigma[i]];

    return result;
}

/**
* \brief Prime factor decomposition
*
* \note Runs in \f$\mathcal{O}(\sqrt{n})\f$ time complexity
*
* \param n Integer different from 0, 1 or -1
* \return Integer vector containing the factors
*/
inline std::vector<bigint> factors(bigint n)
{
    if (n < 0)
        n = -n;

    // EXCEPTION CHECKS

    if (n == 0 || n == 1)
        throw Exception("qpp::factors()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    std::vector<bigint> result;
    bigint d = 2;

    while (n > 1)
    {
        while (n % d == 0)
        {
            result.push_back(d);
            n /= d;
        }
        ++d;
        if (d * d > n) // changes the running time from O(n) to O(sqrt(n))
        {
            if (n > 1)
            {
                result.push_back(n);
            }
            break;
        }
    }

    return result;
}


namespace internal
{
// answer by @danaj
// https://programmingpraxis.com/2013/05/28/modular-multiplication-without-overflow/
inline bigint mulmod(bigint a, bigint b, bigint m)
{
    bigint r = 0;
    /* Remove these mods if the caller can ensure a and b are in range */
    a %= m;
    b %= m;
    while (b > 0)
    {
        if (b & 1) // last bit is one
            r = ((m - r) > a) ? r + a : r + a - m;    /* r = (r + a) % m */
        b >>= 1; // shift right, i.e. divide by 2
        if (b)
            a = ((m - a) > a) ? a + a : a + a - m;    /* a = (a + a) % m */
    }
    return r;
}

// http://www.geeksforgeeks.org/how-to-avoid-overflow-in-modular-multiplication/
// To compute (a * b) % mod
inline bigint mulmod1(bigint a, bigint b, bigint mod)
{
    bigint res = 0; // Initialize result
    a = a % mod;
    while (b > 0)
    {
        // If b is odd, add 'a' to result
        if (b % 2 == 1)
            res = (res + a) % mod;

        // Multiply 'a' with 2
        a = (a * 2) % mod;

        // Divide b by 2
        b /= 2;
    }

    // Return result
    return res % mod;
}
} // end namespace internal

/**
* \brief Fast integer power modulo \a p based on
* the SQUARE-AND-MULTIPLY algorithm
*
* Computes \f$a^n \mathrm{ mod } p\f$
*
* \param a Non-negative integer
* \param n Non-negative integer
* \param p Strictly positive integer
* \return \f$a^n\mathrm{ mod }p\f$
*/
inline bigint modpow(bigint a, bigint n, bigint p)
{
    // EXCEPTION CHECKS

    if (a < 0 || n < 0 || p < 1)
        throw Exception("qpp::modpow()", Exception::Type::OUT_OF_RANGE);

    if (a == 0 && n == 0)
        throw Exception("qpp::modpow()", Exception::Type::OUT_OF_RANGE);

    // END EXCEPTION CHECKS

    if (a == 0 && n > 0)
        return 0;

    if (n == 0 && p == 1)
        return 0;

    bigint result = 1;

    for (; n > 0; n /= 2)
    {
        if (n % 2)
            result = (internal::mulmod1(result, a, p)) % p; // MULTIPLY
        a = (internal::mulmod1(a, a, p)) % p; // SQUARE
    }

    return result;
}

/**
 * \brief Extended greatest common divisor of two integers
 * \see qpp::gcd()
 *
 * \param m Integer
 * \param n Integer
 * \return Tuple of: 1. Integer \f$a\f$, 2. Integer \f$b\f$,
 * and 3. Non-negative integer \f$gcd(a, b)\f$ such that
 * \f$am + bn = gcd(a, b)\f$
 */
inline std::tuple<bigint, bigint, bigint> egcd(bigint m, bigint n)
{
    // EXCEPTION CHECKS

    if (m == 0 && n == 0)
        throw Exception("qpp::egcd()", Exception::Type::OUT_OF_RANGE);

    // END EXCEPTION CHECKS

    bigint a, b, c, q, r;
    bigint a1 = 0, a2 = 1, b1 =1, b2 = 0;

    while (n)
    {
        q = m / n, r = m - q * n;
        a = a2 - q * a1, b = b2 - q * b1;
        m = n, n = r;
        a2 = a1, a1 = a, b2 = b1, b1 = b;
    }
    c = m, a = a2, b = b2;

    // correct the signs
    if (c < 0)
    {
        a = -a;
        b = -b;
        c = -c;
    }

    return std::make_tuple(a, b, c);
}

/**
 * \brief Modular inverse of \a a mod \a p
 * \see qpp::egcd()
 *
 * \note \a a and \a p must be co-prime
 *
 * \param m Non-negative integer
 * \param p Non-negative integer
 * \return Modular inverse \f$a^{-1}\f$ of \a a mod \a p
 */
inline bigint modinv(bigint a, bigint p)
{
    // EXCEPTION CHECKS

    if (a <= 0 || p <= 0)
        throw Exception("qpp::modinv()", Exception::Type::OUT_OF_RANGE);

    bigint x, y;
    bigint gcd_ap;
    std::tie(x, y, gcd_ap) = egcd(p, a);

    if (gcd_ap != 1)
        throw Exception("qpp::modinv()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    return (y > 0) ? y : y + p;
}

/**
 * \brief Primality test based on the Miller-Rabin's algorithm
 *
 * \param n Integer different from 0, 1 or -1
 * \param k Number of iterations. The probability of a
 * false positive is \f$2^{-k}\f$.
 * \return True if the number is (most-likely) prime, false otherwise
 */
inline bool isprime(bigint n, idx k = 80)
{
    if (n < 0)
        n = -n;

    // EXCEPTION CHECKS

    if (n < 2)
        throw Exception("qpp::isprime()", Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    if (n == 2 || n == 3)
        return true;

//    // perform a Fermat primality test
    bigint x = rand(2, n - 1);
    if (modpow(x, n - 1, n) != 1)
        return false;

    // compute u and r
    bigint u = 0, r = 1;

    // write n − 1 as 2^u * r
    for (bigint i = n - 1; i % 2 == 0; ++u, i /= 2);
    r = (n - 1) / static_cast<bigint>(std::llround(std::pow(2, u)));

    // repeat k times
    for (idx i = 0; i < k; ++i)
    {
        // pick a random integer a in the range [2, n − 2]
        bigint a = rand(2, n - 2);

        // set z = a^r mod n
        bigint z = modpow(a, r, n);

        if (z == 1 || z == n - 1)
            continue;

        // repeat u - 1 times
        bool jump = false;
        for (idx j = 0; j < static_cast<idx>(u); ++j)
        {
            z = ((z % n) * (z % n)) % n;
            if (z == 1)
            {
                // composite
                return false;
            }
            if (z == n - 1)
            {
                jump = true;
                break;
            }
        }
        if (jump)
            continue;

        return false;
    }

    return true;
}

/**
* \brief Generates a random big prime uniformly distributed in
* the interval [a, b]
*
* \param a Beginning of the interval, belongs to it
* \param b End of the interval, belongs to it
* \param N Maximum number of candidates
* \return Random big integer uniformly distributed in
* the interval [a, b]
*/
// A std::optional<bigint> return type would have been awesome here!
inline bigint randprime(bigint a, bigint b, idx N = 1000)
{
    // EXCEPTION CHECKS

    if (a > b)
        throw qpp::Exception("qpp::randprime()",
                             qpp::Exception::Type::OUT_OF_RANGE);
    // END EXCEPTION CHECKS

    idx i = 0;
    for (; i < static_cast<bigint>(N); ++i)
    {
        // select a candidate
        bigint candidate = rand(a, b);
        if (std::abs(candidate) < 2)
            continue;
        if (std::abs(candidate) == 2)
            return candidate;

        // perform a Fermat test
        bigint x = rand(2, candidate - 1);
        if (modpow(x, candidate - 1, candidate) != 1)
            continue; // candidate fails

        // passed the Fermat test, perform a Miller-Rabin test
        if (isprime(candidate))
            return candidate;
    }

    if (i == static_cast<bigint>(N))
        throw qpp::Exception("qpp::randprime()", "Prime not found!");

    return 0; // so we don't get a warning
}

} /* namespace qpp */

#endif /* NUMBER_THEORY_H_ */
