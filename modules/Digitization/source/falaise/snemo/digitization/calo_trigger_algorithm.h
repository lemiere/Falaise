// snemo/digitization/calo_trigger_algorithm.h
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>

#ifndef FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_CALO_TRIGGER_ALGORITHM_H
#define FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_CALO_TRIGGER_ALGORITHM_H

// Standard library :
#include <string>
#include <bitset>

// Boost :
#include <boost/circular_buffer.hpp>
#include <boost/scoped_ptr.hpp>

// This project :
#include <snemo/digitization/calo_ctw_data.h>
#include <snemo/digitization/electronic_mapping.h>
#include <snemo/digitization/mapping.h>

namespace datatools {
  class properties;
}

namespace snemo {
  
  namespace digitization {

    /// \brief Calorimeter algorithm general process
    class calo_trigger_algorithm
    {
    public : 

			/// Level one zoning size of a bitset for a zone
			static const int32_t CALO_LEVEL_ONE_MULT_BITSET_SIZE = 2;

			/// Size of the zoning bitset for a side of the calorimeter
			static const int32_t CALO_ZONING_PER_SIDE_BITSET_SIZE = 10;
			
			/// Size of the zoning bitset for gamma veto
			static const int32_t CALO_ZONING_GVETO_BITSET_SIZE = 4;

			/// Size of the information bitset containing LT bit, XT bit and spare bits (up to 4)
			static const int32_t CALO_INFO_BITSET_SIZE = 6;

			struct calo_trigger_record
			{
				uint32_t clocktick_25ns;
				std::bitset<CALO_ZONING_PER_SIDE_BITSET_SIZE> calo_zoning_word[mapping::NUMBER_OF_SIDES];
				std::bitset<CALO_LEVEL_ONE_MULT_BITSET_SIZE> total_calo_multiplicity;
				std::bitset<CALO_ZONING_GVETO_BITSET_SIZE> gveto_zoning_word;
				std::bitset<CALO_INFO_BITSET_SIZE> info_bitset; 
			};
			
			enum calo_zoning_id_index {
				ZONE_0_INDEX = 0,
				ZONE_1_INDEX = 1,
			  ZONE_2_INDEX = 2,
				ZONE_3_INDEX = 3,
				ZONE_4_INDEX = 4,
				ZONE_5_INDEX = 5,
				ZONE_6_INDEX = 6,
				ZONE_7_INDEX = 7,
				ZONE_8_INDEX = 8,
				ZONE_9_INDEX = 9
			};	

			enum calo_zoning_column_id_limits{
				ZONE_0_BEGIN = 0,
				ZONE_0_END   = 1,
				ZONE_1_BEGIN = 2,
				ZONE_1_END   = 3,
			  ZONE_2_BEGIN = 4,
			  ZONE_2_END   = 5,
			  ZONE_3_BEGIN = 6,
			  ZONE_3_END   = 7,
			  ZONE_4_BEGIN = 8,
			  ZONE_4_END   = 9,
			  ZONE_5_BEGIN = 10,
				ZONE_5_END   = 11,
				ZONE_6_BEGIN = 12,
				ZONE_6_END   = 13,
				ZONE_7_BEGIN = 14,
				ZONE_7_END   = 15,
				ZONE_8_BEGIN = 16,
			  ZONE_8_END   = 17,
				ZONE_9_BEGIN = 18,
			  ZONE_9_END   = 19
			};
			
			enum calo_gveto_bitset_pos{
				GVETO_BIT_0 = 6,
				GVETO_BIT_1 = 7,
				GVETO_BIT_2 = 8,
				GVETO_BIT_3 = 9,
				GVETO_BIT_4 = 10,
				GVETO_BIT_5 = 11
			};

			enum calo_info_bitset_pos{
				LT_INFO_BIT        = 0,
				XT_INFO_BIT        = 1,
				CONTROL_INFO_BIT_0 = 2,
				CONTROL_INFO_BIT_1 = 3,
				CONTROL_INFO_BIT_2 = 4,
				CONTROL_INFO_BIT_3 = 5
			};

      /// Default constructor
      calo_trigger_algorithm();

      /// Destructor
      virtual ~calo_trigger_algorithm();

      /// Set the electronic mapping object
      void set_electronic_mapping(const electronic_mapping & my_electronic_mapping_);

			/// Set the calo circular buffer depth
			void set_calo_circular_buffer_depth(unsigned int & calo_circular_buffer_depth_);

      /// Initializing
      void initialize();

      /// Initializing
      void initialize(const datatools::properties & config_);

      /// Check if the algorithm is initialized 
      bool is_initialized() const;

      /// Reset the object
      void reset(); 

      /// Reset private tables
      void reset_trigger_info();

			/// Display the level one calo trigger info (bitsets)
			void display_calo_trigger_info();
    
			/// Build the level one calo trigger primitive bitsets
			void build_calo_level_one_bitsets(const calo_ctw & my_calo_ctw_);

			void build_calo_trigger_record_structure();
			
			/// General process
      void process(const calo_ctw_data & calo_ctw_data_);

		protected :

			/// Protected general process
			void _process(const calo_ctw_data & calo_ctw_data_);

    private :

			typedef boost::circular_buffer<calo_trigger_record> buffer_type;
      // Configuration :
      bool _initialized_; //!< Initialization flag
      const electronic_mapping * _electronic_mapping_; //!< Convert geometric ID into electronic ID
			unsigned int _calo_circular_buffer_depth_;

      // Data :		
     
			bool _level_one_calo_trigger_info_[mapping::NUMBER_OF_SIDES][mapping::NUMBER_OF_CALO_TRIGGER_ZONES]; //!< Table of 2x10 containing 2 bits bitset representing the level one calo trigger info
			
			std::bitset<CALO_LEVEL_ONE_MULT_BITSET_SIZE> _total_calo_multiplicity_; //!< Total multiplicity of calo who passed the HT
			
			std::bitset<CALO_ZONING_GVETO_BITSET_SIZE> _calo_gveto_info_bitset_; //!< Bitset of 4 bits containing the gamma-veto zoning information
			
			std::bitset<CALO_INFO_BITSET_SIZE> _calo_other_info_bitset_; //!< Bitset of 6 bits containing the other bits of all ctw (control, XT, LT)

			boost::scoped_ptr<buffer_type> _calo_gate_circular_buffer_; //!< Scoped pointer to a circular buffer containing output data structure
      
    };

  } // end of namespace digitization

} // end of namespace snemo

#endif // FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_CALO_TRIGGER_ALGORITHM_H

/* 
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/