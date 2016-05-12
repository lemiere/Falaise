// snemo/digitization/trigger_algorithm_test_time.h
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>

#ifndef FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H
#define FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H

// Standard library :
#include <string>
#include <bitset>

// This project :
#include <snemo/digitization/calo_ctw_data.h>
#include <snemo/digitization/geiger_ctw_data.h>
#include <snemo/digitization/calo_trigger_algorithm_test_time.h>
#include <snemo/digitization/tracker_trigger_algorithm_test_time.h>
#include <snemo/digitization/coincidence_trigger_algorithm_test_time.h>
#include <snemo/digitization/electronic_mapping.h>
#include <snemo/digitization/clock_utils.h>
#include <snemo/digitization/mapping.h>
#include <snemo/digitization/trigger_display_manager.h>

namespace datatools {
  class properties;
}    

namespace snemo { 
  
  namespace digitization {

    /// \brief Full trigger algorithm for the process. The trigger decision is taken here.
    class trigger_algorithm_test_time
    {
 		public : 

			enum L2_trigger_mode{
				INVALID   = 0,
				CALO_ONLY = 1,
				CALO_TRACKER_TIME_COINC = 2,
				CARACO  = 3,
				OPEN_DELAYED = 4,
				APE     = 5,
				DAVE    = 6					
			};

			struct L1_calo_decision
			{
				L1_calo_decision();
				void reset();
				const void display() const;
				bool L1_calo_decision_bool;
				uint32_t L1_calo_ct_decision; // CT @ 25 ns
			};

			struct L1_tracker_decision
			{
				L1_tracker_decision();
				void reset();
				const void display() const;
				bool L1_tracker_decision_bool;
				uint32_t L1_tracker_ct_decision; // CT @ 1600 ns
			};
			
			struct L2_decision
			{
				L2_decision();
				void reset();
				const void display() const;
				bool L2_decision_bool;
				uint32_t L2_ct_decision; // CT @ 1600 ns
				trigger_algorithm_test_time::L2_trigger_mode L2_trigger_mode;				
			};

			/// Previous event structure who pass the caraco trigger, store trigger informations (total = 173 [bits]). Useful to search delayed alpha pattern.
			struct previous_event_record
			{
				previous_event_record();
				void reset();
				const void display() const;
				// PER information :
				uint32_t previous_clocktick_1600ns;
				uint32_t counter_1600ns;
				// Calorimeter information :
				std::bitset<trigger_info::NZONES> calo_zoning_word[mapping::NUMBER_OF_SIDES];
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_side_0;
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_side_1;
				bool LTO_side_0;
				bool LTO_side_1;
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_gveto;
				bool LTO_gveto;
				std::bitset<calo_trigger_algorithm_test_time::XT_INFO_BITSET_SIZE> xt_info_bitset;
				bool single_side_coinc;
				bool total_multiplicity_threshold;
				// Tracker information :
				std::bitset<trigger_info::DATA_FULL_BITSET_SIZE> tracker_finale_data_per_zone[trigger_info::NSIDES][trigger_info::NZONES];

				// Coincidence zoning
				std::bitset<trigger_info::NZONES> zoning_word[mapping::NUMBER_OF_SIDES];
			};

			/// Trigger display manager is a friend because it can access to members for display
			friend class trigger_display_manager;
			
      /// Default constructor
      trigger_algorithm_test_time();

      /// Destructor
      virtual ~trigger_algorithm_test_time();

      /// Reset the object
      void reset(); 

			/// Reset internal datas
			void reset_data();

      /// Set the electronic mapping object
      void set_electronic_mapping(const electronic_mapping & my_electronic_mapping_);
			
			/// Set the clock manager object
			void set_clock_manager(const clock_utils & my_clock_manager_);

			/// Check if calorimeter gate size is set
			bool has_calorimeter_gate_size() const;

			/// Set calorimeter gate size
			void set_calorimeter_gate_size(unsigned int calorimeter_gate_size_);

			/// Check if previous event circular buffer depth is set
			bool has_previous_event_buffer_depth() const;

			/// Set previous event circular buffer depth
			void set_previous_event_buffer_depth(unsigned int circular_buffer_depth_);

      /// Check if the coincidence config is activated 
			bool is_activated_coincidence() const;

      /// Initializing
      void initialize();

      /// Initializing
      void initialize(const datatools::properties & config_);

      /// Check if the algorithm is initialized 
      bool is_initialized() const;

			/// Get the vector of calo summary record
			const std::vector<calo_trigger_algorithm_test_time::calo_summary_record> get_calo_records_25ns_vector() const;

			/// Get the vector of coincidence record
			const std::vector<coincidence_trigger_algorithm_test_time::coincidence_calo_record> get_coincidence_calo_records_1600ns_vector() const;

			/// Get the vector of tracker record
			const std::vector<tracker_trigger_algorithm_test_time::tracker_record> get_tracker_records_vector() const;
			
			/// Get the vector of coincidence record
			const std::vector<coincidence_trigger_algorithm_test_time::coincidence_event_record> get_coincidence_records_vector() const;
 			
			/// Get the finale trigger decision
			const bool get_finale_decision() const;	

			/// Get the finale trigger decision
			const bool get_delayed_finale_decision() const;
					
      /// General process
      void process(const calo_ctw_data & calo_ctw_data_,
									 const geiger_ctw_data & geiger_ctw_data_);

    protected :
			
			// /// Process calo algorithm
			// void _process_calo_algo(const calo_ctw_data & calo_ctw_data_);
			
			// /// Process tracker algorithm
			// void _process_tracker_algo(const geiger_ctw_data & geiger_ctw_data_);
			
			// /// Process coincidence module algorithm			
			// void _process_coinc_algo();
			
			
			/// Rescaling calorimeter 25 ns at 1600 ns for coincidences with tracker
			void _rescale_calo_records_at_1600ns(const std::vector<calo_trigger_algorithm_test_time::calo_summary_record> & calo_records_25_ns_,
																					 std::vector<coincidence_trigger_algorithm_test_time::coincidence_calo_record> & coincidence_calo_records_1600ns_);

			/// Update a coinc calo record 1600 from a calo summary record 25 ns
			void _update_coinc_calo_record(const calo_trigger_algorithm_test_time::calo_summary_record & a_calo_summary_record_25ns, 
																		 coincidence_trigger_algorithm_test_time::coincidence_calo_record & a_coinc_calo_record_1600ns);
			
      /// Protected general process
      void _process(const calo_ctw_data & calo_ctw_data_,
										const geiger_ctw_data & geiger_ctw_data_);

    private :
     
			typedef boost::circular_buffer<trigger_algorithm_test_time::previous_event_record> buffer_previous_event_record_type;

      // Configuration :
      bool _initialized_; //!< Initialization flag
      const electronic_mapping * _electronic_mapping_; //!< Convert geometric ID into electronic ID
			const clock_utils * _clock_manager_; //!< Pointer to a clock manager useful for clockticks 
			unsigned int _coincidence_calorimeter_gate_size_; //!< Size of calorimeter gate for extension of calo records during X CT 1600ns
			unsigned int _previous_event_circular_buffer_depth_; //!< Depth for the previous events circular buffer (Pile of PERs)
			
			// Trigger configuration :			
			bool _activate_calorimeter_only_;
			bool _activate_calo_tracker_time_coincidence_;
			bool _activate_caraco_;
			bool _activate_take_all_delayed_;
			bool _activate_ape_dave_coincidence_;
			bool _activate_ape_coincidence_only_;
			bool _activate_any_coincidences_; //!< Boolean activating coincidence

			// Trigger algorithms :
		  tracker_trigger_algorithm_test_time      _tracker_algo_; //!< Tracker trigger algorithm @ 1600ns
		  calo_trigger_algorithm_test_time         _calo_algo_; //!< Calo trigger algorithm @ 25ns
			coincidence_trigger_algorithm_test_time  _coinc_algo_; //!< Coincidence trigger algorithm for matching calo and tracker trigger
			
			// Data :
			std::vector<calo_trigger_algorithm_test_time::calo_summary_record> _calo_records_25ns_; //!< Collection of calo summary record @ 25 ns
			std::vector<coincidence_trigger_algorithm_test_time::coincidence_calo_record> _coincidence_calo_records_1600ns_; //!< Collection of coincidence calo records @ 1600 ns
			std::vector<tracker_trigger_algorithm_test_time::tracker_record> _tracker_records_; //!< Collection of tracker record @ 1600 ns
			std::vector<coincidence_trigger_algorithm_test_time::coincidence_event_record> _coincidence_records_; //!< Collection of coincidence record @ 1600 ns
			boost::scoped_ptr<buffer_previous_event_record_type> _previous_event_records_; //!< Collection of previous event records @ 1600 ns
			std::vector<trigger_algorithm_test_time::L1_calo_decision> _L1_calo_decision_records_; //!< Collection of L1 calorimeter decision @ 25 ns
			std::vector<trigger_algorithm_test_time::L1_tracker_decision> _L1_tracker_decision_records_; //!< Collection of L1 tracker decision @ 1600 ns
			std::vector<trigger_algorithm_test_time::L2_decision> _L2_decision_records_; //!< Collection of L2 decision (which launch the readout)
						
			bool _finale_trigger_decision_; //!< The finale decision for the trigger
			bool _delayed_finale_trigger_decision_; //!< The finale decision for the trigger
			
    };

  } // end of namespace digitization

} // end of namespace snemo

#endif // FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H

/* 
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/