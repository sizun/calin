/* 

   calin/air_shower/geant4_shower_generator.cpp -- Stephen Fegan -- 2015-07-02

   Class to genereate extensive air showers using Geant-4

*/

#include "air_shower/geant4_shower_generator.hpp"
#include "air_shower/geant4_shower_generator_internals.hpp"

using namespace calin::air_shower::shower_generator;
using namespace calin::io::log;

Geant4ShowerGenerator::
Geant4ShowerGenerator(calin::air_shower::tracker::TrackVisitor* visitor,
                      calin::air_shower::atmosphere::Atmosphere* atm,
                      unsigned num_atm_layers, double zground, double ztop,
                      VerbosityLevel verbose_level,
                      bool adopt_visitor, bool adopt_atm):
    visitor_(visitor), adopt_visitor_(adopt_visitor),
    atm_(atm), adopt_atm_(adopt_atm), ztop_of_atm_(ztop), zground_(zground)
{
  // get the pointer to the User Interface manager
  ui_manager_ = G4UImanager::GetUIpointer();

  // construct a session which receives G4cout/G4cerr
  
  calin::io::log::Level cout_level = calin::io::log::VERBOSE;
  calin::io::log::Level cerr_level = calin::io::log::WARNING;
  G4int verbose_everything = 0;
  G4int verbose_event = 0;
  G4int verbose_track = 0;
  switch(verbose_level)
  {
    case VerbosityLevel::SUPPRESSED_ALL:
      cerr_level = calin::io::log::DISCARD;
      // fall through
    case VerbosityLevel::SUPRESSED_STDOUT:
      cout_level = calin::io::log::DISCARD;
      break;
    case VerbosityLevel::NORMAL:
      break;
    case VerbosityLevel::VERBOSE_EVERYTHING:
      verbose_everything = 1;
      // fall through
    case VerbosityLevel::VERBOSE_TRACKING:
      verbose_track = 1;
      // fall through
    case VerbosityLevel::VERBOSE_EVENT:
      verbose_event = 1;
      break;
  }
  ui_session_ = new CoutCerrLogger(cout_level, cerr_level);
  ui_manager_->SetCoutDestination(ui_session_);
  
  // construct the default run manager
  run_manager_ = new G4RunManager;
  
  // set mandatory initialization classes
  FTFP_BERT* physlist = new FTFP_BERT(verbose_everything);
  physlist->SetDefaultCutValue(10*CLHEP::cm);
  physlist->SetVerboseLevel(verbose_everything);
  run_manager_->SetUserInitialization(physlist);

  step_action_ = new EAS_SteppingAction(visitor_);
  run_manager_->SetUserAction(step_action_);

  gen_action_ = new EAS_PrimaryGeneratorAction();
  run_manager_->SetUserAction(gen_action_);

  EAS_FlatDetectorConstruction* detector_constructor =
      new EAS_FlatDetectorConstruction(atm, num_atm_layers, zground, ztop);
  run_manager_->SetUserInitialization(detector_constructor);
  step_action_->setZminCut(zground);


  //run_manager_->SetUserInitialization(new MyUserActionInitialization);

  // initialize G4 kernel
  run_manager_->Initialize();

  // set verbosities
  ui_manager_->
      ApplyCommand(verbose_everything?"/run/verbose 1":"/run/verbose 0");
  ui_manager_->
      ApplyCommand(verbose_event?"/event/verbose 1":"/event/verbose 0");
  ui_manager_->
      ApplyCommand(verbose_track?"/tracking/verbose 1":"/tracking/verbose 0");
}

Geant4ShowerGenerator::~Geant4ShowerGenerator()
{
  delete run_manager_;
  delete ui_session_;
}

void Geant4ShowerGenerator::setMinimumEnergyCut(double emin_mev)
{
  step_action_->setEminCut(emin_mev);
  if(stack_action_ == nullptr)
  {
    stack_action_ = new EAS_StackingAction();
    run_manager_->SetUserAction(stack_action_);
  }
  stack_action_->setEminCut(emin_mev);
}

void Geant4ShowerGenerator::
generateShowers(unsigned num_events,
                calin::air_shower::tracker::ParticleType type,
                double total_energy,
                const Eigen::Vector3d& x0,
                const Eigen::Vector3d& u0,
                double weight)
{
  G4GeneralParticleSource* gps = new G4GeneralParticleSource();

  // default particle kinematic
  G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle
      = particle_table->FindParticle(track_to_pdg_type(type));
 
  G4ThreeVector position;
  eigen_to_g4vec(position, x0, CLHEP::cm);

  G4ThreeVector momentum_direction;
  eigen_to_g4vec(momentum_direction, u0);

  G4SingleParticleSource* sps = gps->GetCurrentSource();
  sps->SetParticleDefinition(particle);
  sps->GetPosDist()->SetPosDisType("Point");
  sps->GetPosDist()->SetCentreCoords(position);
  sps->GetAngDist()->SetAngDistType("planar");
  sps->GetAngDist()->SetParticleMomentumDirection(momentum_direction);
  sps->GetEneDist()->SetEnergyDisType("Mono");
  sps->GetEneDist()->SetMonoEnergy(total_energy * CLHEP::MeV);

  gen_action_->setGPS(gps);
  
  // start a run
  run_manager_->BeamOn(num_events);
}
