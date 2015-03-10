// calo_tp.cc
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>
//

// Ourselves:
#include <snemo/digitization/calo_tp.h>

// Third party:
// - Bayeux/datatools:
//#include <datatools/utils.h>
#include <datatools/exception.h>

namespace snemo {
  
  namespace digitization {

    // Serial tag for datatools::serialization::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(calo_tp, "snemo::digitalization::calo_tp")

    calo_tp::calo_tp()
    {
      _locked_tp_ = false;
      _clocktick_25ns_ = -1;
      _tp_ = 0x0;
      return;
    }

    calo_tp::~calo_tp()
    {
      reset();
      return;
    }

    int32_t calo_tp::get_clocktick_25ns() const
    {
      return _clocktick_25ns_;
    }

    void calo_tp::set_clocktick_25ns( int32_t value_ )
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, "Clocktick can't be set, calorimeter TP is locked !) ");
      if(value_ <= -1)
	{
	  reset_clocktick_25ns();
	}
      else
	{
      _clocktick_25ns_ = value_;
      _store |= STORE_CLOCKTICK_25NS;
	}
      return;
    }

    void calo_tp::reset_clocktick_25ns()
    {
      _clocktick_25ns_ = -1;
      _store &= ~STORE_CLOCKTICK_25NS;
      return;
    }

    const std::bitset<5> & calo_tp::get_tp_bitset() const
    {
      return _tp_;
    }
    
    // std::bitset<5> & calo_tp::grab_tp_bitset()
    // {
    //   _store |= STORE_TP;
    //    return _tp_;
    // }

    // void calo_tp::set_tp_bitset(const std::bitset<5> & tp_)
    // {
    //   DT_THROW_IF(is_locked_tp(), std::logic_error, "TP bitset can't be set, calorimeter TP is locked ! ");
    //   _tp_ = tp_;
    //   _store |= STORE_TP;
    //   return;
    // }
    
    void calo_tp::reset_tp_bitset()
    {
      _tp_ = 0x0;
      _store &= ~STORE_TP;
      return;
    }

    void calo_tp::set_htm_info(unsigned int multiplicity_)
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, "HTM bits can't be set, calorimeter TP is locked ! ");
      switch (multiplicity_)
	{
	case 0 :	  
	  _tp_.set(HTM_BIT0, 0);
	  _tp_.set(HTM_BIT1, 0);
	  break;
	  
	case 1 :
	  _tp_.set(HTM_BIT0, 1);
	  _tp_.set(HTM_BIT1, 0);
	  break;
	  
	case 2 :
	  _tp_.set(HTM_BIT0, 0);
	  _tp_.set(HTM_BIT1, 1);
	  break;
	  
	default :
	  _tp_.set(HTM_BIT0, 1);
	  _tp_.set(HTM_BIT1, 1); 
	}
      _store |= STORE_TP;
      return;
    }
    
    unsigned int calo_tp::get_htm_info() const
    {
      if(_tp_.test(HTM_BIT0 == 0 && HTM_BIT1 == 0))
	{      
	  return 0;
	}
      else if(_tp_.test(HTM_BIT0 == 1 && HTM_BIT1 == 0))
	{
	  return 1;
	}
      else if(_tp_.test(HTM_BIT0 == 0 && HTM_BIT1 == 1))
	{
	  return 2;
	}
      return 3;
    }

    std::bitset<2> calo_tp::get_htm_bits() const
    {
      std::bitset<2> htm_word = 0x0;
      if(_tp_.test(HTM_BIT0) == true)
	{
	  htm_word.set(HTM_BIT0, 1);
	}
      if(_tp_.test(HTM_BIT1) == true)
	{
	  htm_word.set(HTM_BIT1, 1);
	}
      return htm_word;
    }

    bool calo_tp::is_htm() const
    {
      return get_htm_info() != 0;
    }

    void calo_tp::set_lto_bit(bool value_)
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, "LTO bit can't be set, calorimeter TP is locked ! ");
      _tp_.set(LTO_BIT,value_);
      _store |= STORE_TP;
      return;
    }
    
    bool calo_tp::is_lto() const
    {
      return _tp_.test(LTO_BIT);
    }

    void calo_tp::set_xt_bit(bool value_)
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, " External bit can't be set, calorimeter TP is locked ! ");
      _tp_.set(XT_BIT,value_);
      _store |= STORE_TP;
      return;
    }
    
    bool calo_tp::is_xt() const
    {
      return _tp_.test(XT_BIT);
    }

    void calo_tp::set_spare_bit(bool value_)
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, " Spare bit can't be set, calorimeter TP is locked ! ");
      _tp_.set(SPARE_BIT,value_);
      _store |= STORE_TP;
      return;
    }
    
    bool calo_tp::is_spare() const
    {
      return _tp_.test(SPARE_BIT);
    }

    bool calo_tp::is_locked_tp() const
    {
      return _locked_tp_;
    }

    void calo_tp::lock_tp()
    {
      DT_THROW_IF(is_locked_tp(), std::logic_error, "Calorimeter TP is already locked ! ");
      _check();
      _locked_tp_ = true;
      return;
    }
    
    void calo_tp::unlock_tp()
    {
      DT_THROW_IF(!is_locked_tp(), std::logic_error, "Calorimeter TP is already unlocked ! ");
      _locked_tp_ = false;
      return;
    } 

    bool calo_tp::is_valid() const
    {
      return _clocktick_25ns_ >= 0;
    }

    void  calo_tp::reset()
    {
      reset_tp_bitset();
      reset_clocktick_25ns();
      geomtools::base_hit::reset();
      return;
    }

    void calo_tp::tree_dump (std::ostream & out_,
			     const std::string & title_,
			     const std::string & indent_,
			     bool inherit_) const
    {
      base_hit::tree_dump (out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Clock tick (25 ns)  : " << _clocktick_25ns_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag (inherit_)
           << "TP (5 bits) : " << _tp_  << std::endl;
      return;
    }
    
    void calo_tp::_check()
    {
      DT_THROW_IF(!is_valid(), std::logic_error, "Clocktick is not valid ! ");
    }

  } // end of namespace digitization

} // end of namespace snemo