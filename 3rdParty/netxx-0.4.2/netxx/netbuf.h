#ifndef _netxx_netbuf_h_
 #define _netxx_netbuf_h_
 
 // Netxx includes
 #include "streambase.h"
 
 // standard includes
 #include <streambuf>
 #include <algorithm>
 #include <cstring>
 
 namespace {
     const std::streamsize PUTBACK_SIZE = 4;
 }
 
 namespace Netxx {
 
 /**
  * The Netxx::Netbuf template class is a IOStreams streambuf. After you
  * create an instance of a Netxx::Netbuf class you can pass a pointer to it
  * to a std::iostream, std::ostream or std::istream class. Then you can do
  * basic IOStreams operations on it.
  *
  * This streambuf is buffered so you should call std::flush to make sure it
  * sends the data that you inserted.
 **/
 template <std::streamsize bufsize, class charT=char, class traits=std::char_traits<char> >
 class Netbuf : public std::basic_streambuf<charT, traits> {
 public:
     /// int type
     typedef typename std::basic_streambuf<charT, traits>::int_type int_type;
 
     /// char type
     typedef typename std::basic_streambuf<charT, traits>::char_type char_type;
 
     //####################################################################
     /** 
      * Construct a Netxx::Netbuf object and link it to the given StreamBase
      * object. The StreamBase object will be used for reading and writing to
      * the Netbuf (std::streambuf) object.
      *
      * @param stream The StreamBase object to use for reading and writing.
      * @author Peter Jones
     **/
     //####################################################################
     explicit Netbuf (StreamBase &stream);
 
     //####################################################################
     /** 
      * Netxx::Netbuf class destructor.
      *
      * @author Peter Jones
     **/
     //####################################################################
     ~Netbuf (void);
 protected:
     // TODO streamsize xsputn (const char_type *s, streamsize n);
     int_type overflow (int_type c=traits::eof());
     int sync (void);
 
     int_type underflow (void);
     int_type pbackfail (int_type c);
 private:
     StreamBase &stream_;
     charT putbuf_[bufsize];
     charT getbuf_[bufsize];
 
     int buffer_out (void);
     int buffer_in  (void);
 
     Netbuf (const Netbuf&);
     Netbuf& operator= (const Netbuf&);
 }; // end Netxx::Netbuf class
 
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 Netbuf<bufsize, charT, traits>::Netbuf (StreamBase &stream)
     : stream_(stream)
 {
     setp(putbuf_, putbuf_ + bufsize);
     setg(getbuf_+PUTBACK_SIZE, getbuf_+PUTBACK_SIZE, getbuf_+PUTBACK_SIZE);
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 Netbuf<bufsize, charT, traits>::~Netbuf (void) {
     sync();
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 typename Netbuf<bufsize, charT, traits>::int_type Netbuf<bufsize, charT, traits>::overflow (int_type c) {
     if (buffer_out() < 0) {
         return traits::eof();
     } else if (!traits::eq_int_type(c, traits::eof())) {
         return sputc(c);
     } else {
         return traits::not_eof(c);
     }
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 int Netbuf<bufsize, charT, traits>::sync (void) {
     return buffer_out();
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 int Netbuf<bufsize, charT, traits>::buffer_out (void) {
     int length = this->pptr() - this->pbase();
     int rc = stream_.write(putbuf_, length);
     this->pbump(-length);
     return rc;
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 typename Netbuf<bufsize, charT, traits>::int_type Netbuf<bufsize, charT, traits>::underflow (void) {
     if (this->gptr() < this->egptr()) return traits::to_int_type(*this->gptr());
     if (buffer_in() < 0) return traits::eof();
     else return traits::to_int_type(*this->gptr());
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 typename Netbuf<bufsize, charT, traits>::int_type Netbuf<bufsize, charT, traits>::pbackfail(int_type c) {
     if (this->gptr() != this->eback()) {
         this->gbump(-1);
 
         if (!traits::eq_int_type(c, traits::eof())) {
             *(this->gptr()) = traits::to_char_type(c);
         }
 
         return traits::not_eof(c);
     } else {
         return traits::eof();
     }
 }
 //#############################################################################
 template<std::streamsize bufsize, class charT, class traits>
 int Netbuf<bufsize, charT, traits>::buffer_in (void) {
     std::streamsize number_putbacks = std::min(this->gptr() - this->eback(), PUTBACK_SIZE);
     std::memcpy(getbuf_ + (PUTBACK_SIZE - number_putbacks) * sizeof(char_type),
             this->gptr() - number_putbacks * sizeof(char_type), number_putbacks * sizeof(char_type));
 
     int rc = stream_.read(getbuf_ + PUTBACK_SIZE * sizeof(char_type), bufsize - PUTBACK_SIZE);
     
     if (rc <= 0) {
         this->setg(0, 0, 0);
         return -1;
     } else {
         setg(getbuf_ + PUTBACK_SIZE - number_putbacks, getbuf_ + PUTBACK_SIZE, getbuf_ + PUTBACK_SIZE + rc);
         return rc;
     }
 }
 
 } // end Netxx namespace
 #endif
