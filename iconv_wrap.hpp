// iconv_wrap.hpp --- iconv wrapper class
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#ifndef ICONV_WRAP_HPP_
#define ICONV_WRAP_HPP_     2   // Version 2

#include <iconv.h>

//////////////////////////////////////////////////////////////////////////////

class iconv_wrap
{
public:
    iconv_wrap();
    iconv_wrap(const char *to, const char *from);
    virtual ~iconv_wrap();

    bool open(const char *to, const char *from);
    bool is_open() const;
    void close();

    operator iconv_t() const;

    bool convert(const void *input, size_t *in_left,
                 void *output, size_t *out_left);
    void reset();

protected:
    iconv_t m_cd;
};

//////////////////////////////////////////////////////////////////////////////

inline iconv_wrap::iconv_wrap() : m_cd((iconv_t)-1)
{
}

inline iconv_wrap::iconv_wrap(const char *to, const char *from)
{
    m_cd = iconv_open(to, from);
}

inline iconv_wrap::~iconv_wrap()
{
    close();
}

inline bool iconv_wrap::open(const char *to, const char *from)
{
    close();
    m_cd = iconv_open(to, from);
}

inline bool iconv_wrap::is_open() const
{
    return (m_cd != (iconv_t)-1);
}

inline void iconv_wrap::close()
{
    if (is_open())
    {
        iconv_close(m_cd);
        m_cd = (iconv_t)-1;
    }
}

inline iconv_wrap::operator iconv_t() const
{
    return m_cd;
}

inline bool
iconv_wrap::convert(const void *input, size_t *in_left,
                    void *output, size_t *out_left)
{
    if (!is_open())
        return false;

    char *in = (char *)input;
    char *out = (char *)output;
    size_t ret = iconv(m_cd, &in, in_left, &out, out_left);
    if (ret == (size_t)-1)
        return false;

    return true;
}

inline void iconv_wrap::reset()
{
    iconv(m_cd, NULL, NULL, NULL, NULL);
}

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef ICONV_WRAP_HPP_
