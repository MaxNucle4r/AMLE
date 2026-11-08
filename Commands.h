#pragma once
#include <unordered_map>
#include <string_view>





enum class Command
{
    //special commands
    Help,
    Cls,
    Version,


    Generate,
    Print,
    Train,
    Model,
    Config,
    Debug,
    Clear,

    None
};

std::string_view commandToString(Command command);



static const std::unordered_map<std::string_view, Command> commands =
{
    {"help", Command::Help},

    {"generate", Command::Generate},
    {"print", Command::Print},
    {"train", Command::Train},
    {"model", Command::Model},
    {"config", Command::Config},
    {"clear", Command::Clear},

    // for @[command]{ [options] } commands types
    {"debug", Command::Debug }
};


static const std::unordered_map<std::string_view, Command> specialCommands =
{
    {"help", Command::Help},
    {"cls", Command::Cls},
    {"version", Command::Version}
};





enum class Target
{
    Text,
    Word,
    Memory,
    Vocab,
    Transitions,
    Probs,
    Save,
    Load,
    Files,
    Input,

    //self train for later
    Self,

    //for @config
    Generation,
    UI,
    Runtime,

    //for @debug
    DebugFlags,


    None
};

std::string_view targetToString(Target target);



static const std::unordered_map<std::string_view, Target> targets =
{
    {"text", Target::Text},
    {"word", Target::Word},
    {"memory", Target::Memory},
    {"vocab", Target::Vocab},
    {"transitions", Target::Transitions},
    {"probs", Target::Probs},
    {"save", Target::Save},
    {"load", Target::Load},
    {"files", Target::Files,},
    {"input", Target::Input},
    {"flags", Target::DebugFlags},
    {"generation", Target::Generation},
    {"runtime", Target::Runtime},
    {"ui", Target::UI},
    {"self", Target::Self}

};





enum class OptionType
{
    NONE,

	String,
	Int,
    SizeT,
	Float,
	Bool
	
};

enum class Option
{
    NONE,

    Model,
    NgramSize,


	// for non Ngram related shit
    Size,

    Temperature,

    //for @print
    Text, Char,

	//for @generate
    Seed,

	DebugFlag,

	//defaults from @config
	D_Temperature,
	D_MaxLength,
	D_Model,
	D_NgramSize,
    
    //useless now
	D_AutoSaveMinutes,
    D_AutoSave,
    

	//more config options
	//also not added yet
    ShowTiming,
	ColoredOutput,


	Filename,
};

Option stringToOption(std::string_view optionName);



namespace
{
    struct OptionDefinition
    {
        std::string_view name;
        std::string_view description;

        Option option;
        OptionType type;
    };

}



using Options = std::unordered_map<Option, OptionDefinition>;
using Targets = std::unordered_map<Target, Options>;
using CommandRegistry = std::unordered_map<Command, Targets>;

inline const  CommandRegistry commandRegistry =
{

    {
        Command::Cls,
        {
            {
                Target::None,
                {
                    {
                        Option::NONE,
                        {   "[none]", 
                            "Will clear the screen.",
                            Option::NONE, OptionType::NONE
                        }

                    }

                }

            }

        }

    },



    {
        //TO DO
        // finish @help command
        // it should also maybe be able to take options
        // @help{generate} to spit out info abt @generate
        Command::Help,
        {
            {
                Target::None,
                {
                    {
                        Option::NONE,
                        {   "[none]", 
                            "Will display information about available ALL commands.",
                            Option::NONE, OptionType::NONE
                        }

                    }

                }

            }

        }

    },



    {
        Command::Version,
        {
            {
                Target::None,
                {
                    {
                        Option::NONE,
                        {   "[none]", 
                            "Will print version related information.",
                            Option::NONE, OptionType::NONE
                        }

                    }

                }

            }


        }

    },










    {
        Command::Generate,
        {
            {
                Target::Text,
                {
                    {

                        Option::NONE,
                        {   "[none]",
                            "Will generate a text using the default settings.",
                            Option::NONE, OptionType::NONE
                        },

                    },


                    {
                        Option::Temperature,
                        {   "temperature",
                            "The temperature to use for generation. Higher values will result in more random output. "
                            "The option value must be a decimal number between 0.1 and 2.5 ",
                            Option::Temperature, OptionType::Float
                        },

                    },


                    {
                        Option::Model,
                        {   "model",
                            "The model to use for generation. "
                            "The option value must be 'ngram' or 'legacy'",
                            Option::Model, OptionType::String
                        },

                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The size of the n-grams to use for generation. Only applicable if the model is 'ngram'. "
                            "The option value must be a number greater than or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        }
                    },


                    {
                        Option::Seed,
                        {   "seed",
                            "The seed to use for generation. "
                            "The option value must be a string",
                            Option::Seed, OptionType::String
                        }


                    }


                }



            },





            {
                Target::Word,
                {
                    {

                        Option::NONE,
                        {   "[none]",
                            "Will generate a word using the default settings.",
                            Option::NONE, OptionType::NONE
                        },

                    },


                    {
                        Option::Temperature,
                        {   "temperature",
                            "The temperature to use for generation. Higher values will result in more random output. "
                            "The option value must be a decimal number between 0.1 and 2.5 ",
                            Option::Temperature, OptionType::Float
                        },

                    },


                    {
                        Option::Model,
                        {   "model",
                            "The model to use for generation. "
                            "The option value must be 'ngram' or 'legacy'",
                            Option::Model, OptionType::String
                        },

                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The size of the n-grams to use for generation. Only applicable if the model is 'ngram'. "
                            "The option value must be a number greater than or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        }


                    }



                }


            }


        },

    },
    









    {
        Command::Print,
        {
            {
                Target::Transitions,
                {   
                    
                    {
						Option::NONE,
                        {   "[none]",
                            "Will print the transitions for all models.",
                            Option::NONE, OptionType::NONE
                        },
                        
                    },


                    {
						Option::Model,
						{   "model",
							"The model to print transitions for. Can be either 'ngram' or 'legacy'. "
                            "The option value must be 'ngram' or 'legacy'",
							Option::Model, OptionType::String
						},

                    },


                    {
						Option::Text,
						{   "text",
							"Will print the text transitions for all models. "
                            "The option value must be true/false",
							Option::Text, OptionType::Bool
						},

                    },


                    {
						Option::Char,
						{   "char",
							"Will print the character transitions for all models. "
							"The option value must be true/false",
							Option::Char, OptionType::Bool
						},


                    },


                    {
						Option::NgramSize,
                        {   "ngram-size",
                            "The size of the n-grams to print transitions for. Only applicable if the model is 'ngram'. "
                            "The option value must be a number greater than or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        }
                        
                    },
        


                },
                


            },





            {

				Target::Probs,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will print the probabilities for all models.",
                            Option::NONE, OptionType::NONE
                        },

                    },


                    {
                        Option::Model,
                        {   "model",
                            "The model to print probabilities for. Can be either 'ngram' or 'legacy'. "
                            "The option value must be 'ngram' or 'legacy'",
                            Option::Model, OptionType::String
                        },

                    },


                    {
                        Option::Text,
                        {   "text",
                            "Will print the text probabilities for all models. "
                            "The option value must be true/false",
                            Option::Text, OptionType::Bool
                        },

                    },


                    {
                        Option::Char,
                        {   "char",
                            "Will print the character probabilities for all models. "
                            "The option value must be true/false",
                            Option::Char, OptionType::Bool
                        },


                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The size of the n-grams to print probabilities for. Only applicable if the model is 'ngram'. "
                            "The option value must be a number greater than or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        }

                    },


                }


            },





            {
                    
                Target::Memory,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will print the memory.",
                            Option::NONE, OptionType::NONE
                        },
                    },


                    {
						Option::Size,
						{   "size",
							"The number of entries to print from the memory. "
							"The option value must be a non-zero number. ",
							Option::Size, OptionType::SizeT
						},
                        
                    },

                }

                
            },





			{

				Target::Vocab,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will print the vocabulary.",
                            Option::NONE, OptionType::NONE
                        },
                    },


                }


            }


        }



    },

    








    {
        Command::Train,
        {
            {
				Target::Input,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will train the ALL models using the input data.",
                            Option::NONE, OptionType::NONE
                        },
                    },


                    {
                        Option::Model,
                        {   "model",
                            "Will train the specified model using the input data. "
                            "The option value must be either 'legacy' or 'ngram' or 'both'",
                            Option::Model, OptionType::String
                        },
                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The size of the n-grams to use for training. "
                            "The option value must be a number greater than or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        },
                    },



                }
				
            }


        }


    },










    {
		Command::Model,
        {

            {
                Target::Save,
                {   
                    
                    {
						Option::NONE,
						{   "[none]",
							"Will save the model to a file using a default name. "
                            "The default name will be '[timestamp].amle'",
							Option::NONE, OptionType::NONE
						},
                        
                    },


                    {
                        Option::Filename,
						{   "filename",
							"The name of the file to save the model to. "
							"The option value must be a string.",
							Option::Filename, OptionType::String
                        },

                    },
                
                },

            },





            {
                Target::Load,
                {
                       
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will load the model from a file using a default name. "
                            "Upon not recieving a filename, it will use the LAST MODEL FETCHED.",
                            Option::NONE, OptionType::NONE
                        }
                    },


                    {
                        Option::Filename,
                        {   "filename",
                            "The name of the file to load the model from. "
                            "The option value must be a string.",
                            Option::Filename, OptionType::String
                        },

                    }

                }

            },





            {
                Target::Files,
                {
                    {
                        Option::NONE,
                        {   "[none]",
							"Will print the names of all saved models.",
                            Option::NONE, OptionType::NONE
                        }
                    },


                },

            },



        },


    
    },










    {
		Command::Config,
        {
            {
                Target::Generation,
                {
                    { 
                        Option::D_Temperature,
                        {
                            "d-temp",
                            "The default temperature to use for generation. "
                            "Higher values will result in more random output.",
                            Option::D_Temperature, OptionType::Float
                        }
                    
                    },


                    {
                        Option::D_Model,
                        {
							"d-model",
							"The default model to use for generation. "
							"The option value must be either 'legacy' or 'ngram'.",
							Option::D_Model, OptionType::String
                        }

                    },


                    {
                        Option::D_MaxLength,
                        {
                            //TO DO
                            // turn this into a size_t as well
							"d-max-length",
							"W.I.P",
							Option::D_MaxLength, OptionType::Int
                        }

                    },


                    {
                        Option::D_NgramSize,
                        {
							"d-ngram-size",
							"The default n-gram size to use for generation. "
                            "The option value must be a number greater than or equal to 2.",
							Option::D_NgramSize, OptionType::SizeT
                        }

                    },

                }
            
            },





            {
				Target::UI,
                {
                    {
                        Option::ColoredOutput,
                        {
                            //TO DO
                            "colored-output",
                            "W.I.P",
                            Option::ColoredOutput, OptionType::Bool
                        }
                    },


					{
						Option::ShowTiming,
						{
							//TO DO
							"show-timing",
							"W.I.P",
							Option::ShowTiming, OptionType::Bool
						}
					},


                 }
            
            
            },





            {
				Target::Runtime,
                {
                    {
                        Option::D_AutoSave,
                        {
                            "auto-save",
                            "Whether to automatically save the model every X minutes. "
                            "Currently not fully implemented.",
                            Option::D_AutoSave, OptionType::Bool
                        }
                    },


                    {
                        Option::D_AutoSaveMinutes,
                        {
                            // TO DO
							// make this a size_t instead of an int
                            "auto-save-minutes",
                            "The interval at which to automatically save the model. "
							"Must be greater than 0. "
                            "Currently not fully implemented.",
                            Option::D_AutoSaveMinutes, OptionType::Int
                        }
                    },

                }
            
            },
        
        
        }
    
    },










    {
        Command::Debug,
        {
            {
                Target::DebugFlags,
                {
                    {
						Option::DebugFlag,
						{
                            //the flags will be printed from the debug flags map in Debug.h
							"[flag]",
							"The debug flag to enable or disable. "
							"The option value must be a boolean. ",
                            Option::DebugFlag, OptionType::Bool
							
						}
                    }
                    
                }
                   
            
            }
        
        }
    
    },


    









    {
    
		Command::Clear,
		{
			{
				Target::Self,
				{
                    {
						Option::NONE,
						{   "[none]",
							"Will clear the entire model.",
							Option::NONE, OptionType::NONE
						}

                    }
				}

			},





            {
                Target::Transitions,
                {
                    {
					    Option::NONE,
						{   "[none]",
							"Will clear the transitions AND probabilities for all models.",
							Option::NONE, OptionType::NONE
						},
					},


                    {
						Option::Model,
						{   "model",
							"The model to clear transitions AND probabilities for. "
							"The option value must be 'ngram' or 'legacy'",
							Option::Model, OptionType::String
						},
                    
                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The n-gram size for the model to clear transitions AND probabilities for. "
                            "The option value must be a positive number larger or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        },

                    },

               
                }

            },





            {
                Target::Probs,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will clear the probabilities for all models.",
                            Option::NONE, OptionType::NONE
                        },
                    },


                    {
                        Option::Model,
                        {   "model",
                            "The model to clear probabilities for. "
                            "The option value must be 'ngram' or 'legacy'",
                            Option::Model, OptionType::String
                        },

                    },


                    {
                        Option::NgramSize,
                        {   "ngram-size",
                            "The n-gram size for the model to clear probabilities for. "
                            "The option value must be a positive number larger or equal to 2.",
                            Option::NgramSize, OptionType::SizeT
                        },

                    },  


                }

            },




            {
				Target::Memory,
                {
                    {
                        Option::NONE,
                        {   "[none]",
                            "Will clear the memory.",
                            Option::NONE, OptionType::NONE
                        
                        }
                    }

                }
                

            
            },





            {
				Target::Vocab,
				{
					{
						Option::NONE,
						{   "[none]",
							"Will clear the vocabulary.",
							Option::NONE, OptionType::NONE

						}

					}

				}

            
            },


		}

    
    }



};